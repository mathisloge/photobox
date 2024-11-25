#include "CollageContext.hpp"
#include <fstream>
#include <Pbox/CleanupAsyncScope.hpp>
#include <Pbox/Logger.hpp>
#include "CollagePrinter.hpp"
#include "ImageStorage.hpp"

DEFINE_LOGGER(collage_context)

namespace
{
void logError(std::exception_ptr ex)
{
    try
    {
        if (ex)
        {
            std::rethrow_exception(ex);
        }
    }
    catch (const std::exception &e)
    {
        LOG_ERROR(collage_context, "Error while initializing the collage: {}", e.what());
    }
}
} // namespace
namespace Pbox
{
CollageContext::CollageContext(Scheduler &scheduler,
                               ImageStorage &image_storage,
                               std::filesystem::path collage_directory,
                               std::optional<std::filesystem::path> printer_settings)
    : scheduler_{scheduler}
    , image_storage_{image_storage}
    , system_status_client_{QStringLiteral("Collage"), true}
{
    if (printer_settings.has_value())
    {
        printer_required_ = true;
        // The printer initialization might need some time.
        auto init_printer = stdexec::schedule(scheduler.getWorkScheduler()) |
                            stdexec::then([this, printer_settings = std::move(*printer_settings)]() {
                                printer_ = std::make_unique<CollagePrinter>(printer_settings);
                                updateSystemStatus();
                            }) |
                            stdexec::upon_error([this](auto &&ex_ptr) {
                                system_status_client_.setSystemStatus(SystemStatusCode::Code::Error);
                                logError(std::forward<decltype(ex_ptr)>(ex_ptr));
                            });
        async_scope_.spawn(std::move(init_printer));
    }

    auto init_collage =
        // important to put the fonts into the svg render thread.
        stdexec::schedule(scheduler_.getSvgRenderScheduler()) |  //
        stdexec::then([this]() { init_lunasvg(font_cache_); }) | //
        stdexec::continues_on(scheduler_.getWorkScheduler()) |
        stdexec::then([this, collage_directory = std::move(collage_directory)]() {
            std::ifstream settings_file{collage_directory / "collage_settings.json"};
            nlohmann::json json;
            settings_file >> json;
            settings_ = json;

            renderer_.loadDocument(collage_directory / "collage.svg");
            for (auto &&element : settings_.image_elements)
            {
                renderer_.addPhotoElement(element);
            }
            document_ready_ = true;
            updateSystemStatus();
        }) |
        stdexec::upon_error([this](auto &&ex_ptr) {
            system_status_client_.setSystemStatus(SystemStatusCode::Code::Error);
            logError(std::forward<decltype(ex_ptr)>(ex_ptr));
        });

    async_scope_.spawn(std::move(init_collage));
}

CollageContext::~CollageContext()
{
    cleanup_async_scope(async_scope_);
}

void CollageContext::updateSystemStatus()
{
    if (system_status_client_.systemStatus() == SystemStatusCode::Code::Error or
        system_status_client_.systemStatus() == SystemStatusCode::Code::Ok)
    {
        return;
    }
    const bool printer_ready = (printer_required_ and printer_ != nullptr) or not printer_required_;
    if (document_ready_ and printer_ready)
    {
        system_status_client_.setSystemStatus(SystemStatusCode::Code::Ok);
    }
}

std::filesystem::path CollageContext::saveAndPrintCollage()
{
    const auto saved_path = image_storage_.storageDir() / image_storage_.generateNewImageFilePath();
    renderer_.updateLayout();
    renderer_.renderToFile(saved_path);
    if (printer_ != nullptr)
    {
        printer_->print(renderer_);
    }
    return saved_path;
}

Scheduler &CollageContext::scheduler()
{
    return scheduler_;
}

CollageRenderer &CollageContext::renderer()
{
    return renderer_;
}

const CollageSettings &CollageContext::settings() const
{
    return settings_;
}

const SystemStatusClient &CollageContext::systemStatusClient()
{
    return system_status_client_;
}

} // namespace Pbox

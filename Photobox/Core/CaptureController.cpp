// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CaptureController.hpp"
#include <fstream>
#include <Pbox/CleanupAsyncScope.hpp>
#include <Pbox/Conditional.hpp>
#include <Pbox/Logger.hpp>
#include <fmt/format.h>
#include "CameraImageProvider.hpp"
#include "CollagePrinter.hpp"
#include "CollageRenderer.hpp"
#include "ICamera.hpp"
#include "ImageStorage.hpp"

DEFINE_LOGGER(captureControllerLog)

namespace Pbox
{
CaptureController::CaptureController(Scheduler &scheduler,
                                     const std::filesystem::path &collage_directory,
                                     std::unique_ptr<ImageStorage> image_storage,
                                     std::shared_ptr<ICamera> camera,
                                     std::unique_ptr<CollagePrinter> printer)
    : scheduler_{scheduler}
    , image_storage_{std::move(image_storage)}
    , camera_{std::move(camera)}
    , printer_{std::move(printer)}
    , collage_renderer_{std::make_unique<CollageRenderer>()}
{
    Q_ASSERT(image_storage_ != nullptr);
    Q_ASSERT(camera_ != nullptr);
    Q_ASSERT(collage_renderer_ != nullptr);

    connect(camera_.get(), &ICamera::imageCaptured, this, &CaptureController::saveImage);
    loadSettings(collage_directory);
}

CaptureController::~CaptureController()
{
    cleanup_async_scope(async_scope_);
}

void CaptureController::captureImage()
{
    camera_->requestCapturePhoto();
    if (current_capture_ == max_capture_count_)
    {
        collage_finished_ = true;
        Q_EMIT collageCompletedChanged();
    }
}

void CaptureController::reset()
{
    collage_finished_ = false;
    current_capture_ = 0;
    capture_model_.resetImageCount(max_capture_count_);
    Q_EMIT collageCompletedChanged();
}

CaptureImageModel *CaptureController::getModel()
{
    return std::addressof(capture_model_);
}

CameraImageProvider *CaptureController::createImageProvider()
{
    auto *image_provider = new CameraImageProvider();
    // TODO: release images after collage is finished. => Do this in CaptureSessions
    connect(this, &CaptureController::imageCaptured, image_provider, &CameraImageProvider::addImage);
    return image_provider;
}

bool CaptureController::isCollageComplete() const
{
    return collage_finished_;
}

QString CaptureController::getCollageImagePath()
{
    if (collage_image_path_.empty())
    {
        return "";
    }
    return QString::fromStdString(std::filesystem::absolute(collage_image_path_));
}

void CaptureController::loadSettings(const std::filesystem::path &collage_directory)
{
    async_scope_.spawn(stdexec::schedule(scheduler_.getSvgRenderScheduler()) |
                       stdexec::then([this]() { init_lunasvg(font_cache_); }));

    async_scope_.spawn(stdexec::schedule(scheduler_.getWorkScheduler()) //
                       | stdexec::then([this, collage_directory]() {
                             std::ifstream settings_file{collage_directory / "collage_settings.json"};
                             nlohmann::json json;
                             settings_file >> json;
                             settings_ = json;

                             collage_renderer_->loadDocument(collage_directory / "collage.svg");
                             for (auto &&element : settings_.image_elements)
                             {
                                 collage_renderer_->addPhotoElement(element);
                             }
                             max_capture_count_ = static_cast<int>(settings_.image_elements.size()) - 1;
                         }));
}

void CaptureController::saveImage(const QImage &image)
{
    const auto unique_image_name =
        QString::fromStdString(fmt::format("capture-{}-{}", current_capture_, image_counter_++));
    Q_EMIT imageCaptured(image, unique_image_name);
    capture_model_.setImage(current_capture_, unique_image_name);
    Q_EMIT capturedImageReady();

    auto image_save_flow =
        stdexec::schedule(scheduler_.getWorkScheduler()) | stdexec::then([this, image]() {
            const auto image_name = image_storage_->generateNewImageFilePath();

            const auto image_path = image_storage_->storageDir() / image_name;
            LOG_DEBUG(captureControllerLog, "Saving image to {}", image_path.string());
            if (not image.save(QString::fromStdString(image_path)))
            {
                LOG_ERROR(captureControllerLog, "Could not save image to {}", image_path.string());
            }
            return image_path;
        }) |
        stdexec::then([this](const std::string &captured_image_path) {
            const auto &element = settings_.image_elements.at(current_capture_);
            collage_renderer_->setSourceOfPhoto(element, captured_image_path);
            return captured_image_path;
        }) |
        stdexec::let_value([this](const std::string &image_path) {
            return conditional(
                collage_finished_,
                stdexec::schedule(scheduler_.getSvgRenderScheduler()) | stdexec::then([this, image_path]() {
                    collage_image_path_ = image_storage_->storageDir() / image_storage_->generateNewImageFilePath();
                    LOG_DEBUG(captureControllerLog, "saving collage to {}", collage_image_path_.string());
                    collage_renderer_->updateLayout();
                    collage_renderer_->dumpAsJson(fmt::format("{}.json", collage_image_path_.string()));
                    collage_renderer_->renderToFile(collage_image_path_);
                    printer_->print(*collage_renderer_);
                }) | stdexec::continues_on(scheduler_.getQtEventLoopScheduler()) |
                    stdexec::then([this]() { Q_EMIT collageCaptureComplete(); }),
                stdexec::schedule(scheduler_.getQtEventLoopScheduler()) |
                    stdexec::then([this]() { current_capture_++; }));
        });

    async_scope_.spawn(std::move(image_save_flow));
}

int CaptureImageModel::rowCount(const QModelIndex & /*parent*/) const
{
    return static_cast<int>(image_sources_.size());
}

QVariant CaptureImageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= image_sources_.size())
    {
        return {};
    }
    switch (Roles{role})
    {
    case Roles::ImageSource:
        return image_sources_.at(index.row());
    }
    return {};
}

QHash<int, QByteArray> CaptureImageModel::roleNames() const
{
    static const auto kRoles = QHash<int, QByteArray>{
        {std::underlying_type_t<Roles>(Roles::ImageSource), "source"},
    };
    return kRoles;
}

void CaptureImageModel::resetImageCount(int count)
{
    beginResetModel();
    image_sources_.clear();
    image_sources_.reserve(count);
    endResetModel();
}

void CaptureImageModel::setImage(int element_index, QString source)
{
    if (image_sources_.size() <= element_index)
    {
        beginInsertRows({}, image_sources_.size(), image_sources_.size());
        image_sources_.emplace_back(std::move(source));
        endInsertRows();
    }
    else
    {
        image_sources_.at(element_index) = std::move(source);
        Q_EMIT dataChanged(index(element_index), index(element_index));
    }
}

QString CaptureImageModel::sourceOfLastItem() const
{
    if (image_sources_.empty())
    {
        return "";
    }
    return image_sources_.back();
}
} // namespace Pbox

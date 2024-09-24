#include "CaptureController.hpp"
#include <fstream>
#include <fmt/format.h>
#include "CameraImageProvider.hpp"
#include "CollagePrinter.hpp"
#include "CollageRenderer.hpp"
#include "ICamera.hpp"
#include "ImageStorage.hpp"
namespace Pbox
{
namespace
{
class CollageSaveWorkerThread : public QThread
{
    Q_OBJECT
  public:
    CollageSaveWorkerThread(std::filesystem::path file_path, CollageRenderer &renderer, CollagePrinter &printer)
        : file_path_{std::move(file_path)}
        , renderer_{renderer}
        , printer_{printer}
    {}

  private:
    void run() override
    {
        renderer_.dumpAsJson(fmt::format("{}.json", file_path_.c_str()));
        renderer_.renderToFile(file_path_);
        printer_.print(renderer_);
    }

  private:
    std::filesystem::path file_path_;
    CollageRenderer &renderer_;
    CollagePrinter &printer_;
};
} // namespace

CaptureController::CaptureController(const std::filesystem::path &collage_directory,
                                     std::unique_ptr<ImageStorage> image_storage,
                                     std::shared_ptr<ICamera> camera,
                                     std::unique_ptr<CollagePrinter> printer)
    : image_storage_{std::move(image_storage)}
    , camera_{std::move(camera)}
    , printer_{std::move(printer)}
    , collage_renderer_{std::make_unique<CollageRenderer>()}
{
    Q_ASSERT(image_storage_ != nullptr);
    Q_ASSERT(camera_ != nullptr);
    Q_ASSERT(collage_renderer_ != nullptr);

    connect(camera_.get(), &ICamera::imageCaptured, image_storage_.get(), &ImageStorage::onImageCaptured);
    connect(camera_.get(), &ICamera::imageCaptured, this, [this](const QImage &image) {
        const auto unique_image_name =
            QString::fromStdString(fmt::format("capture-{}-{}", current_capture_, image_counter_++));

        Q_EMIT imageCaptured(image, unique_image_name);
        capture_model_.setImage(current_capture_, unique_image_name);
    });
    connect(this, &CaptureController::captureComplete, this, [this]() {
        if (not collage_finished_)
        {
            current_capture_++;
        }
    });
    connect(image_storage_.get(),
            &ImageStorage::imageSaved,
            this,
            [this](const std::filesystem::path &captured_image_path) {
                const auto &element = settings_.image_elements.at(current_capture_);
                collage_renderer_->setSourceOfPhoto(element, captured_image_path);
                if (collage_finished_)
                {
                    collage_image_path_ =
                        image_storage_->storageDir() / fmt::format("collage_{}.png", image_counter_++);
                    collage_renderer_->updateLayout();
                    CollageSaveWorkerThread *worker_thread =
                        new CollageSaveWorkerThread(collage_image_path_, *collage_renderer_, *printer_);
                    connect(worker_thread,
                            &CollageSaveWorkerThread::finished,
                            this,
                            &CaptureController::collageCaptureComplete);
                    connect(worker_thread, &CollageSaveWorkerThread::finished, worker_thread, &QObject::deleteLater);
                    worker_thread->start();
                }
                else
                {
                    Q_EMIT captureComplete();
                }
            });

    loadSettings(collage_directory);
}

CaptureController::~CaptureController() = default;

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
}

CaptureImageModel *CaptureController::getModel()
{
    return std::addressof(capture_model_);
}

CameraImageProvider *CaptureController::createImageProvider()
{
    auto *image_provider = new CameraImageProvider();
    connect(this, &CaptureController::imageCaptured, image_provider, &CameraImageProvider::addImage);
    return image_provider;
}

bool CaptureController::isCollageComplete() const
{
    return collage_finished_;
}

QString CaptureController::getCollageImagePath()
{
    return QString::fromStdString(collage_image_path_);
}

void CaptureController::loadSettings(const std::filesystem::path &collage_directory)
{
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
} // namespace Pbox
#include "CaptureController.moc"
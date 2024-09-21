#include "CaptureController.hpp"
#include "CameraImageProvider.hpp"
#include "CollageRenderer.hpp"
#include "ICamera.hpp"
#include "ImageStorage.hpp"

namespace Pbox
{
CaptureController::CaptureController(std::unique_ptr<ImageStorage> image_storage, std::shared_ptr<ICamera> camera)
    : image_storage_{std::move(image_storage)}
    , camera_{std::move(camera)}
    , collage_renderer_{std::make_unique<CollageRenderer>()}
{
    Q_ASSERT(image_storage_ != nullptr);
    Q_ASSERT(camera_ != nullptr);
    Q_ASSERT(collage_renderer_ != nullptr);

    collage_renderer_->loadDocument("/home/mathis/Downloads/TestImage.svg");
    collage_renderer_->addPhotoElement("image-1");
    collage_renderer_->addPhotoElement("image-2");
    collage_renderer_->addPhotoElement("image-3");
    image_count_collage_ = static_cast<int>(collage_renderer_->registeredImages().size()) - 1;

    connect(camera_.get(), &ICamera::imageCaptured, image_storage_.get(), &ImageStorage::onImageCaptured);
    connect(camera_.get(), &ICamera::imageCaptured, this, [this](const QImage &image) {
        const QString unique_image_name =
            QStringLiteral("capture-%1-%2").arg(current_image_count_).arg(image_counter_++);

        Q_EMIT imageCaptured(image, unique_image_name);
        capture_model_.setImage(current_image_count_, unique_image_name);
        qDebug() << "CAPTURE COUNT current_image_count_ =" << current_image_count_
                 << "image_count_collage_ =" << image_count_collage_;
        if (current_image_count_ == image_count_collage_)
        {
            qDebug() << "capture complete";
            Q_EMIT collageCaptureComplete();
        }
        else
        {
            current_image_count_++;
        }
    });
    connect(image_storage_.get(),
            &ImageStorage::imageSaved,
            this,
            [this](const std::filesystem::path &captured_image_path) {
                collage_renderer_->setSourceOfPhoto(
                    QStringLiteral("image-%1").arg(current_image_count_ + 1).toStdString(), captured_image_path);
                if (current_image_count_ == image_count_collage_)
                {
                    collage_renderer_->updateLayout();
                    collage_renderer_->renderToFile();
                }
            });
}

CaptureController::~CaptureController() = default;

void CaptureController::captureImage()
{
    camera_->requestCapturePhoto();
}

void CaptureController::reset()
{
    current_image_count_ = 0;
    capture_model_.resetImageCount(image_count_collage_);
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

int CaptureImageModel::rowCount(const QModelIndex & /*parent*/) const
{
    return static_cast<int>(image_sources_.size());
}

QVariant CaptureImageModel::data(const QModelIndex &index, int role) const
{
    qDebug() << "TEST" << index.row();
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
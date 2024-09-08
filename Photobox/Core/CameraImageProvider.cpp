#include "CameraImageProvider.hpp"
#include "ICamera.hpp"

using namespace Qt::Literals::StringLiterals;

namespace Pbox
{
CameraImageProvider::CameraImageProvider(std::shared_ptr<ICamera> camera)
    : QQuickImageProvider{QQuickImageProvider::Pixmap}
    , camera_{std::move(camera)}
{
    connect(camera_.get(), &ICamera::imageCaptured, this, [this](const QImage &image) {
        qDebug() << "test";
        last_captured_image_ = QPixmap::fromImage(image);
    });
}

CameraImageProvider::~CameraImageProvider() = default;

QPixmap CameraImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug() << "image requested: " << id;
    if (id == "capture"_L1)
    {
        if (requestedSize.isValid() && requestedSize != last_captured_image_.size())
        {
            auto scaled_image = last_captured_image_.scaled(requestedSize, Qt::AspectRatioMode::KeepAspectRatio);
            size->setWidth(scaled_image.width());
            size->setHeight(scaled_image.height());
            return scaled_image;
        }

        size->setWidth(last_captured_image_.width());
        size->setHeight(last_captured_image_.height());
        return last_captured_image_;
    }
    return {};
}
} // namespace Pbox

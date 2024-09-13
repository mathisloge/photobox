#include "CameraImageProvider.hpp"
#include <QDateTime>
#include "ICamera.hpp"

using namespace Qt::Literals::StringLiterals;

namespace Pbox
{
CameraImageProvider::CameraImageProvider(std::shared_ptr<ICamera> camera)
    : QQuickImageProvider{QQuickImageProvider::Pixmap}
    , camera_{std::move(camera)}
{
    connect(camera_.get(), &ICamera::imageCaptured, this, [this](const QImage &image) {
        qDebug() << "start CameraImageProvider::imageCaptured" << QDateTime::currentDateTime();
        last_captured_image_ = QPixmap::fromImage(image);
        qDebug() << "end CameraImageProvider::imageCaptured" << QDateTime::currentDateTime();
    });
}

CameraImageProvider::~CameraImageProvider() = default;

QPixmap CameraImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    if (id == "capture"_L1)
    {
        qDebug() << "start pixmap" << QDateTime::currentDateTime();
        if (requestedSize.isValid() && requestedSize != last_captured_image_.size())
        {
            qDebug() << "resize pixmap" << QDateTime::currentDateTime();
            auto scaled_image = last_captured_image_.scaled(requestedSize, Qt::AspectRatioMode::KeepAspectRatio);
            size->setWidth(scaled_image.width());
            size->setHeight(scaled_image.height());
            qDebug() << "ret pixmap" << QDateTime::currentDateTime();
            return scaled_image;
        }

        size->setWidth(last_captured_image_.width());
        size->setHeight(last_captured_image_.height());

        qDebug() << "ret pixmap" << QDateTime::currentDateTime();
        return last_captured_image_;
    }
    return {};
}
} // namespace Pbox

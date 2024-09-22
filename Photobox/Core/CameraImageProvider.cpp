#include "CameraImageProvider.hpp"

using namespace Qt::Literals::StringLiterals;

namespace Pbox
{
CameraImageProvider::CameraImageProvider()
    : QQuickImageProvider{QQuickImageProvider::Pixmap}
{}

void CameraImageProvider::addImage(const QImage &image, QString id)
{
    captured_images_[std::move(id)] = QPixmap::fromImage(image);
}

void CameraImageProvider::resetCache()
{
    captured_images_.clear();
}

QPixmap CameraImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    const auto it = captured_images_.find(id);
    if (it != captured_images_.end())
    {
        const auto &image = it->second;
        if (requestedSize.isValid() && requestedSize != image.size())
        {
            auto scaled_image = image.scaled(requestedSize, Qt::AspectRatioMode::KeepAspectRatio);
            size->setWidth(scaled_image.width());
            size->setHeight(scaled_image.height());
            return scaled_image;
        }

        size->setWidth(image.width());
        size->setHeight(image.height());

        return image;
    }
    return {};
}
} // namespace Pbox

// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathis.opensource@tuta.io>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ImageProvider.hpp"

using namespace Qt::Literals::StringLiterals;

namespace Pbox
{
ImageProvider::ImageProvider()
    : QQuickImageProvider{QQuickImageProvider::Pixmap}
{}

ImageProvider::~ImageProvider() = default;

void ImageProvider::addImage(const QImage &image, std::uint32_t id)
{
    captured_images_[id] = QPixmap::fromImage(image);
}

void ImageProvider::resetCache()
{
    captured_images_.clear();
}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    bool converted{false};
    std::uint32_t image_id = id.toUInt(std::addressof(converted));
    if (not converted)
    {
        return {};
    }

    const auto it = captured_images_.find(image_id);
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

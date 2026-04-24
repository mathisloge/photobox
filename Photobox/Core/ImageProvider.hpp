// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QQuickImageProvider>
#include <Pbox/DisableCopyMove.hpp>

namespace Pbox
{
/**
 * @brief QML image provider for captured photos.
 *
 * This class provides captured images to QML components with on-demand scaling.
 * Images are cached by ID and can be requested with optional size constraints.
 */
class ImageProvider : public QQuickImageProvider
{
    Q_OBJECT
  public:
    PBOX_DISABLE_COPY_MOVE(ImageProvider);
    ImageProvider();
    ~ImageProvider() override;

    /**
     * @brief Adds an image to the cache.
     * @param image The image to cache.
     * @param id The unique ID for the image.
     */
    void addImage(const QImage &image, std::uint32_t id);
    /**
     * @brief Clears all cached images.
     */
    void resetCache();
    /**
     * @brief Requests an image by ID with optional scaling.
     * @param id The image ID.
     * @param size Output parameter for the image size.
     * @param requestedSize The requested size (may be scaled).
     * @return The pixmap of the image.
     */
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

  private:
    std::unordered_map<std::uint32_t, QPixmap> captured_images_; /**< Cached images by ID. */
};
} // namespace Pbox

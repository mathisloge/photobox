// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QQuickImageProvider>
#include <Pbox/DisableCopyMove.hpp>

namespace Pbox
{
class ImageProvider : public QQuickImageProvider
{
    Q_OBJECT
  public:
    PBOX_DISABLE_COPY_MOVE(ImageProvider);
    ImageProvider();
    ~ImageProvider() override;

    void addImage(const QImage &image, std::uint32_t id);
    void resetCache();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;

  private:
    std::unordered_map<std::uint32_t, QPixmap> captured_images_;
};
} // namespace Pbox

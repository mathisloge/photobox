// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QImage>
#include <QObject>
#include <Pbox/DisableCopyMove.hpp>
#include <exec/async_scope.hpp>
#include "ICaptureSession.hpp"

namespace Pbox
{
class CollageContext;
class CollageCaptureSession : public ICaptureSession
{
    Q_OBJECT
  public:
    PBOX_DISABLE_COPY_MOVE(CollageCaptureSession);
    explicit CollageCaptureSession(CollageContext &context);
    ~CollageCaptureSession() override;
    void triggerCapture() override;
    void imageCaptured(const QImage &captured_image, std::uint32_t image_id) override;
    void imageSaved(const std::filesystem::path &captured_image_path) override;

  private:
    void startCountdownOrFinish();
    void handleCountdown(int current_count);
    void handlePreviewTimeout();
    void finish();
    bool allImagesCaptured() const;

  private:
    exec::async_scope async_scope_;
    CollageContext &context_;
    int current_capture_{0};
    bool preview_visible_{true};
    bool finished_{false};
    QTimer preview_timer_;
    std::optional<std::filesystem::path> saved_collage_path_;
};
} // namespace Pbox

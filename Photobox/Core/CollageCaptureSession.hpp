// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
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

    /// vvv property methods
    bool isCountdownVisible() const override;
    const QString &getCountdownText() const override;
    /// ^^^ property methods

  Q_SIGNALS:
    void finished();

  private:
    void startCountdownOrFinish();
    void handleCountdown();
    void handlePreviewTimeout();
    void finish();
    bool allImagesCaptured() const;

  private:
    exec::async_scope async_scope_;
    CollageContext &context_;
    int current_capture_{0};
    bool preview_visible_{true};
    int countdown_counter_{0};
    bool finished_{false};
    QString final_countdown_text_{QStringLiteral("Smile!")};
    QString current_countdown_text_;
    QTimer countdown_timer_;
    QTimer preview_timer_;
    std::optional<std::filesystem::path> saved_collage_path_;
};
} // namespace Pbox

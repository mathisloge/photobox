// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "ICaptureSession.hpp"

namespace Pbox
{
class SingleCaptureSession : public ICaptureSession
{
    Q_OBJECT
  public:
    SingleCaptureSession();
    void triggerCapture() override;
    void imageCaptured(const QImage &captured_image, std::uint32_t image_id) override;
    void imageSaved(const std::filesystem::path &captured_image_path) override;

    bool isCountdownVisible() const override;
    const QString &getCountdownText() const override;

  private:
    void startCapturing();
    void handleCountdown();

  private:
    int countdown_counter_{};
    QString countdown_text_;

    QTimer countdown_timer_;
};
} // namespace Pbox

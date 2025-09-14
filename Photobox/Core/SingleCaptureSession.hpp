// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
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

  private:
    void startCapturing();
    void handleCountdown(int count);

  private:
};
} // namespace Pbox

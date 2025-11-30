// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QCamera>
#include <QImageCapture>
#include <QMediaCaptureSession>
#include <QVideoFrame>
#include "ICamera.hpp"
#include "SystemStatusClient.hpp"

namespace Pbox
{
class MockCamera : public ICamera
{
    Q_OBJECT
  public:
    MockCamera();
    ~MockCamera() override;
    Q_DISABLE_COPY_MOVE(MockCamera);

    void requestCapturePhoto() override;

    const SystemStatusClient &systemStatusClient() const override;

  private:
    SystemStatusClient status_client_;
    QMediaCaptureSession capture_session_;
    QImageCapture image_capture_;
    QCamera camera_;
};
} // namespace Pbox

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
/**
 * @brief Mock camera implementation using Qt's media framework.
 *
 * This class provides a test camera implementation that uses the system's
 * webcam or test video source for development and testing purposes.
 */
class MockCamera : public ICamera
{
    Q_OBJECT
  public:
    MockCamera();
    ~MockCamera() override;
    Q_DISABLE_COPY_MOVE(MockCamera);

    /**
     * @brief Requests a photo capture from the mock camera.
     */
    void requestCapturePhoto() override;

    /**
     * @brief Gets the system status client.
     * @return Reference to the status client.
     */
    const SystemStatusClient &systemStatusClient() const override;

  private:
    SystemStatusClient status_client_;     /**< Status client for the camera. */
    QMediaCaptureSession capture_session_; /**< Qt media capture session. */
    QImageCapture image_capture_;          /**< Qt image capture device. */
    QCamera camera_;                       /**< Qt camera object. */
};
} // namespace Pbox

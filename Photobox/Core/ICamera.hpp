// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <QVideoSink>

namespace Pbox
{
class SystemStatusClient;

/**
 * @brief Abstract interface for camera implementations.
 *
 * This class defines the interface for camera backends (e.g., gphoto2, mock)
 * to capture photos and provide live video preview.
 */
class ICamera : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVideoSink *videoSink READ getVideoSink WRITE setVideoSink NOTIFY videoSinkChanged FINAL)
  public:
    ICamera() = default;
    ~ICamera() override = default;
    Q_DISABLE_COPY_MOVE(ICamera);

    /**
     * @brief Gets the current video sink.
     * @return Pointer to the video sink.
     */
    QVideoSink *getVideoSink() const;
    /**
     * @brief Sets the video sink for live preview.
     * @param video_sink The video sink to set.
     */
    void setVideoSink(QVideoSink *video_sink);

    /**
     * @brief Gets the system status client for this camera.
     * @return Reference to the status client.
     */
    virtual const SystemStatusClient &systemStatusClient() const = 0;

    /**
     * @brief Requests a photo capture.
     */
    Q_INVOKABLE virtual void requestCapturePhoto() = 0;
  Q_SIGNALS:
    /**
     * @brief Emitted when the video sink changes.
     */
    void videoSinkChanged();

    /**
     * @brief Emitted when an image is captured.
     * @param captured_image The captured image.
     */
    void imageCaptured(const QImage &captured_image);

  private:
    QVideoSink *video_sink_{nullptr};
};
} // namespace Pbox

// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathis.opensource@tuta.io>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <QVideoSink>

namespace Pbox
{
class SystemStatusClient;

class ICamera : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVideoSink *videoSink READ getVideoSink WRITE setVideoSink NOTIFY videoSinkChanged FINAL)
  public:
    ICamera() = default;
    ~ICamera() override = default;
    Q_DISABLE_COPY_MOVE(ICamera);

    QVideoSink *getVideoSink() const;
    void setVideoSink(QVideoSink *video_sink);

    virtual const SystemStatusClient &systemStatusClient() const = 0;

    Q_INVOKABLE virtual void requestCapturePhoto() = 0;
  Q_SIGNALS:
    void videoSinkChanged();

    void imageCaptured(const QImage &captured_image);

  private:
    QVideoSink *video_sink_{nullptr};
};
} // namespace Pbox

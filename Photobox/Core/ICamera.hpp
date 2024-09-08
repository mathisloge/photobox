#pragma once
#include <QObject>
#include <QVideoSink>

namespace Pbox
{
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

    Q_INVOKABLE virtual void requestCapturePhoto() = 0;
  Q_SIGNALS:
    void videoSinkChanged();

    void imageCaptured(const QImage &captured_image);

  private:
    QVideoSink *video_sink_{nullptr};
};
} // namespace Pbox

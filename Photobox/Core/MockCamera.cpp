#include "MockCamera.hpp"
#include <QDebug>
#include <QMediaDevices>
#include <QPointer>
#include <QTimer>
#include <QVideoFrameFormat>
namespace Pbox
{
MockCamera::MockCamera()
    : camera_{QMediaDevices::defaultVideoInput()}
{
    capture_session_.setCamera(&camera_);
    capture_session_.setImageCapture(&image_capture_);
    camera_.setActive(true);

    connect(this, &ICamera::videoSinkChanged, this, [this]() { capture_session_.setVideoSink(getVideoSink()); });

    connect(&image_capture_, &QImageCapture::imageCaptured, this, [this](auto, auto &&preview) {
        qDebug() << "got image";
        QTimer::singleShot(std::chrono::seconds{1}, this, [this, preview]() {
            qDebug() << "publish image";
            Q_EMIT imageCaptured(preview);
        });
    });
}
MockCamera::~MockCamera() = default;

void MockCamera::requestCapturePhoto()
{
    image_capture_.capture();
}
} // namespace Pbox

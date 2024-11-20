#include "MockCamera.hpp"
#include <QDebug>
#include <QMediaDevices>
#include <QPointer>
#include <QTimer>
#include <QVideoFrameFormat>
namespace Pbox
{
MockCamera::MockCamera()
    : status_client_(QStringLiteral("Mock Kamera"), true)
    , camera_{QMediaDevices::defaultVideoInput()}
{
    capture_session_.setCamera(&camera_);
    capture_session_.setImageCapture(&image_capture_);

    connect(&camera_, &QCamera::errorOccurred, this, [this](QCamera::Error error, QString /*message*/) {
        status_client_.setSystemStatus(error == QCamera::Error::NoError ? SystemStatusCode::Code::Ok
                                                                        : SystemStatusCode::Code::Error);
    });
    status_client_.setSystemStatus(SystemStatusCode::Code::Ok);

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

const SystemStatusClient &MockCamera::systemStatusClient() const
{
    return status_client_;
}
} // namespace Pbox

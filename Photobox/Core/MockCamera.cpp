// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "MockCamera.hpp"
#include <QDebug>
#include <QMediaDevices>
#include <QPointer>
#include <QTimer>
#include <QVideoFrameFormat>
#include <Pbox/Logger.hpp>

DEFINE_LOGGER(mock_camera)

namespace Pbox
{
MockCamera::MockCamera()
    : status_client_(QStringLiteral("Mock Camera"), true)
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
        LOG_INFO(mock_camera, "Got image. Delaying publishing...");
        QTimer::singleShot(std::chrono::seconds{3}, this, [this, preview]() {
            LOG_INFO(mock_camera, "Publishing image...");
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

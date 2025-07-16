// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SingleCaptureSession.hpp"
#include <Pbox/Logger.hpp>
#include <fmt/core.h>

DEFINE_LOGGER(single_capture_session)

namespace Pbox
{
SingleCaptureSession::SingleCaptureSession()
    : ICaptureSession{"SingleCaptureSession"}
{
    getCountdown()->setSeconds(6);
    connect(getCountdown(), &Countdown::finished, this, &SingleCaptureSession::requestedImageCapture);
    connect(getCountdown(), &Countdown::currentCountChanged, this, &SingleCaptureSession::handleCountdown);
}

void SingleCaptureSession::triggerCapture()
{
    if (getStatus() == ICaptureSession::Status::Idle)
    {
        setLiveViewVisible(true);
        startCapturing();
    }
}

void SingleCaptureSession::imageCaptured(const QImage & /*captured_image*/, std::uint32_t image_id)
{
    setCaptureStatus(ICaptureSession::CaptureStatus::Idle);
    setPreviewImage(QString::fromStdString(fmt::format("image://preview-image/{}", image_id)));
    QTimer::singleShot(std::chrono::seconds{6}, this, &SingleCaptureSession::finished);
}

void SingleCaptureSession::imageSaved(const std::filesystem::path &captured_image_path)
{}

void SingleCaptureSession::startCapturing()
{
    setLiveViewVisible(true);
    setStatus(ICaptureSession::Status::Capturing);
    getCountdown()->start();
}

void SingleCaptureSession::handleCountdown(int count)
{
    LOG_DEBUG(single_capture_session, "Countdown {}", count);
    if (count == 1)
    {
        setLiveViewVisible(false);
        setCaptureStatus(ICaptureSession::CaptureStatus::BeforeCapture);
    }
}
} // namespace Pbox

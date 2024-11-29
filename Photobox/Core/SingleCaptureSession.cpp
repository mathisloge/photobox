// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
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
    countdown_timer_.setTimerType(Qt::TimerType::PreciseTimer);
    countdown_timer_.setInterval(std::chrono::seconds{1});
    countdown_timer_.setSingleShot(false);
    connect(&countdown_timer_, &QTimer::timeout, this, &SingleCaptureSession::handleCountdown);
}

void SingleCaptureSession::triggerCapture()
{
    if (getStatus() == ICaptureSession::Status::Idle)
    {
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

bool SingleCaptureSession::isCountdownVisible() const
{
    return not countdown_text_.isEmpty();
}

const QString &SingleCaptureSession::getCountdownText() const
{
    return countdown_text_;
}

void SingleCaptureSession::startCapturing()
{
    setLiveViewVisible(true);
    setStatus(ICaptureSession::Status::Capturing);
    countdown_counter_ = 5;
    handleCountdown();
    countdown_timer_.start();
}

void SingleCaptureSession::handleCountdown()
{
    countdown_text_ = QString::number(countdown_counter_);
    LOG_DEBUG(single_capture_session, "Countdown {}", countdown_counter_);
    if (countdown_counter_ == 1)
    {
        setCaptureStatus(ICaptureSession::CaptureStatus::BeforeCapture);
    }
    else if (countdown_counter_ == 1)
    {
        countdown_text_ = "Smile!";
    }
    else if (countdown_counter_ == 0)
    {
        countdown_text_ = "";
        countdown_timer_.stop();
        setCaptureStatus(ICaptureSession::CaptureStatus::Idle);
        Q_EMIT requestedImageCapture();
    }
    countdown_counter_--;
    Q_EMIT countdownTextChanged();
    Q_EMIT countdownVisibleChanged();
}
} // namespace Pbox

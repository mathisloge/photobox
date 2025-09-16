// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CollageCaptureSession.hpp"
#include <Pbox/CleanupAsyncScope.hpp>
#include <Pbox/Logger.hpp>
#include <fmt/core.h>
#include "CollageContext.hpp"

DEFINE_LOGGER(collage_capture_session);

namespace Pbox
{

CollageCaptureSession::CollageCaptureSession(Instance<ImageStorage> image_storage,
                                             Instance<CollageRenderer> renderer,
                                             Instance<Scheduler> scheduler,
                                             CollageSettings settings)
    : ICaptureSession("CollageCaptureSession")
    , image_storage_{std::move(image_storage)}
    , renderer_{std::move(renderer)}
    , scheduler_{std::move(scheduler)}
    , settings_{std::move(settings)}
{
    getCountdown()->setSeconds(settings_.seconds_between_capture);
    connect(getCountdown(), &Countdown::finished, this, [this] {
        LOG_DEBUG(logger_collage_capture_session(), "requesting capture...");
        Q_EMIT requestedImageCapture();
    });
    connect(getCountdown(), &Countdown::currentCountChanged, this, &CollageCaptureSession::handleCountdown);

    preview_timer_.setTimerType(Qt::TimerType::PreciseTimer);
    preview_timer_.setInterval(std::chrono::seconds{5});
    preview_timer_.setSingleShot(true);
    connect(&preview_timer_, &QTimer::timeout, this, &CollageCaptureSession::handlePreviewTimeout);
    setLiveViewVisible(true);
}

CollageCaptureSession::~CollageCaptureSession()
{
    cleanup_async_scope(async_scope_);
}

void CollageCaptureSession::handleCountdown(int current_count)
{
    LOG_DEBUG(logger_collage_capture_session(), "Countdown {}", current_count);
    if (current_count <= 1)
    {
        setCaptureStatus(ICaptureSession::CaptureStatus::BeforeCapture);
        setLiveViewVisible(false);
    }
}

void CollageCaptureSession::handlePreviewTimeout()
{
    LOG_DEBUG(logger_collage_capture_session(), "preview timer timeout");
    setPreviewImage({});
    startCountdownOrFinish();
}

void CollageCaptureSession::imageCaptured(const QImage & /*captured_image*/, std::uint32_t image_id)
{
    setCaptureStatus(ICaptureSession::CaptureStatus::Idle);
    setPreviewImage(QString::fromStdString(fmt::format("image://preview-image/{}", image_id)));
    preview_timer_.start();
}

void CollageCaptureSession::imageSaved(const std::filesystem::path &captured_image_path)
{
    try
    {
        auto &&image_element = settings_.image_elements.at(current_capture_);
        renderer_->setSourceOfPhoto(image_element, captured_image_path);
        current_capture_++;
        // finish the collage to do the heavy work while the preview image from the current image is shown.
        if (allImagesCaptured())
        {
            LOG_DEBUG(logger_collage_capture_session(), "Got all images. Finishing...", current_capture_);
            finish();
        }
    }
    catch (const std::out_of_range &ex)
    {
        LOG_ERROR(logger_collage_capture_session(),
                  "Tried to add an image which wasn't part of the collage. current_capture={}. Error: {}",
                  current_capture_,
                  ex.what());
    }
}

void CollageCaptureSession::startCountdownOrFinish()
{
    if (not allImagesCaptured())
    {
        LOG_DEBUG(logger_collage_capture_session(), "Starting countdown");
        setStatus(ICaptureSession::Status::Capturing);
        setLiveViewVisible(true);
        getCountdown()->start();
    }
    else if (not finished_ and saved_collage_path_.has_value())
    {
        LOG_DEBUG(logger_collage_capture_session(), "(startCountdownOrFinish) collage wasn't shown yet.");
        setPreviewImage(QString::fromStdString(fmt::format("file://{}", saved_collage_path_->string())));
        finished_ = true;
        preview_timer_.start();
    }
    else if (finished_)
    {
        LOG_DEBUG(logger_collage_capture_session(), "(startCountdownOrFinish) collage is now finished.");
        Q_EMIT finished();
    }
    else
    {
        LOG_WARNING(logger_collage_capture_session(),
                    "some weird state. allImagesCaptured={}, saved_collage_path_.has_value()={}, finished_={}",
                    allImagesCaptured(),
                    saved_collage_path_.has_value(),
                    finished_);
    }
}

void CollageCaptureSession::finish()
{
    setStatus(ICaptureSession::Status::Busy);

    async_scope_.spawn(
        stdexec::schedule(scheduler_->getSvgRenderScheduler()) | //
        stdexec::then([this]() {
            const auto saved_path = image_storage_->storageDir() / image_storage_->generateNewImageFilePath();
            renderer_->updateLayout();
            renderer_->renderToFile(saved_path);
            return saved_path;
        }) |
        stdexec::continues_on(scheduler_->getQtEventLoopScheduler()) | //
        stdexec::then([this](auto &&saved_image_path) {
            saved_collage_path_ = std::filesystem::absolute(saved_image_path);
            LOG_DEBUG(logger_collage_capture_session(), "collage finished. Saved to {}", saved_collage_path_->string());
            setStatus(ICaptureSession::Status::Capturing);
            startCountdownOrFinish();
        }));
}

void CollageCaptureSession::triggerCapture()
{
    if (getStatus() == Status::Idle)
    {
        startCountdownOrFinish();
    }
}

bool CollageCaptureSession::allImagesCaptured() const
{
    return current_capture_ == settings_.image_elements.size();
}

} // namespace Pbox

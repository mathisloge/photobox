#include "CollageCaptureSession.hpp"
#include <Pbox/CleanupAsyncScope.hpp>
#include <Pbox/Logger.hpp>
#include <fmt/core.h>
#include "CollageContext.hpp"

DEFINE_LOGGER(collage_capture_session);

namespace Pbox
{

CollageCaptureSession::CollageCaptureSession(CollageContext &context)
    : context_{context}
    , countdown_counter_(context_.settings().seconds_between_capture)
{
    countdown_timer_.setTimerType(Qt::TimerType::PreciseTimer);
    countdown_timer_.setInterval(std::chrono::seconds{1});
    countdown_timer_.setSingleShot(false);
    connect(&countdown_timer_, &QTimer::timeout, this, &CollageCaptureSession::handleCountdown);

    preview_timer_.setTimerType(Qt::TimerType::PreciseTimer);
    preview_timer_.setInterval(std::chrono::seconds{5});
    preview_timer_.setSingleShot(true);
    connect(&preview_timer_, &QTimer::timeout, this, &CollageCaptureSession::handlePreviewTimeout);
}

CollageCaptureSession::~CollageCaptureSession()
{
    cleanup_async_scope(async_scope_);
}

void CollageCaptureSession::handleCountdown()
{
    countdown_counter_--;
    if (countdown_counter_ == 0)
    {
        setLiveViewVisible(false);
        current_countdown_text_ = final_countdown_text_;
    }
    else if (countdown_counter_ < 0)
    {
        countdown_counter_ = context_.settings().seconds_between_capture;
        current_countdown_text_ = QString{};
        countdown_timer_.stop();
        LOG_DEBUG(collage_capture_session, "requesting capture...");
        Q_EMIT requestedImageCapture();
    }
    else
    {
        current_countdown_text_ = QString::number(countdown_counter_);
    }
    LOG_DEBUG(collage_capture_session, "Countdown {}: {}", countdown_counter_, current_countdown_text_.toStdString());
    Q_EMIT countdownTextChanged();
}

void CollageCaptureSession::handlePreviewTimeout()
{
    setPreviewImage({});
    startCountdownOrFinish();
}

void CollageCaptureSession::imageCaptured(const QImage & /*captured_image*/, std::uint32_t image_id)
{
    setPreviewImage(QString::fromStdString(fmt::format("image://preview-image/{}", image_id)));
    preview_timer_.start();
}

void CollageCaptureSession::imageSaved(const std::filesystem::path &captured_image_path)
{
    try
    {
        auto &&image_element = context_.settings().image_elements.at(current_capture_);
        context_.renderer().setSourceOfPhoto(image_element, captured_image_path);
        current_capture_++;
    }
    catch (const std::out_of_range &ex)
    {
        LOG_BACKTRACE(collage_capture_session,
                      "Tried to add an image which wasn't part of the collage. current_capture={}. Error: {}",
                      current_capture_,
                      ex.what());
    }
}

void CollageCaptureSession::startCountdownOrFinish()
{
    if (current_capture_ == context_.settings().image_elements.size())
    {
        LOG_DEBUG(collage_capture_session, "Got all images. Finishing...", current_capture_);
        finish();
    }
    else
    {
        LOG_DEBUG(collage_capture_session, "Starting countdown");
        setStatus(ICaptureSession::Status::Capturing);
        setLiveViewVisible(true);
        countdown_timer_.start();
    }
}

void CollageCaptureSession::finish()
{
    setStatus(ICaptureSession::Status::Busy);
    auto finish = stdexec::continues_on(context_.scheduler().getQtEventLoopScheduler()) |
                  stdexec::then([this](auto &&saved_image_path) {
                      LOG_DEBUG(collage_capture_session,
                                "collage finished. Saved to  {}",
                                std::filesystem::absolute(saved_image_path).string());
                      setPreviewImage(QString::fromStdString(
                          fmt::format("file://{}", std::filesystem::absolute(saved_image_path).string())));
                      setStatus(ICaptureSession::Status::Capturing);
                      Q_EMIT finished();
                  });
    async_scope_.spawn(context_.asyncSaveAndPrintCollage() | std::move(finish));
}

void CollageCaptureSession::triggerCapture()
{
    if (not countdown_timer_.isActive() and not preview_timer_.isActive())
    {
        startCountdownOrFinish();
    }
}

bool CollageCaptureSession::isCountdownVisible() const
{
    return not current_countdown_text_.isEmpty();
}

const QString &CollageCaptureSession::getCountdownText() const
{
    return current_countdown_text_;
}

} // namespace Pbox

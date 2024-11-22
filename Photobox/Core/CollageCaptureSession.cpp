#include "CollageCaptureSession.hpp"
#include <Pbox/Logger.hpp>
#include "CollageContext.hpp"

DEFINE_LOGGER(collage_capture_session);

namespace Pbox
{

CollageCaptureSession::CollageCaptureSession(CollageContext &context)
    : context_{context}
    , countdown_counter_(context.settings().seconds_between_capture)
{
    countdown_timer_.setTimerType(Qt::TimerType::PreciseTimer);
    countdown_timer_.setInterval(std::chrono::seconds{1});
    countdown_timer_.setSingleShot(false);
    connect(&countdown_timer_, &QTimer::timeout, this, &CollageCaptureSession::handleCountdown);
    countdown_timer_.start();
}

CollageCaptureSession::~CollageCaptureSession() = default;

void CollageCaptureSession::handleCountdown()
{
    countdown_counter_--;
    if (countdown_counter_ == 0)
    {
        setPreviewVisible(false);
        current_countdown_text_ = final_countdown_text_;
    }
    else if (countdown_counter_ < 0)
    {
        current_countdown_text_ = QString{};
        countdown_timer_.stop();
        Q_EMIT requestedImageCapture();
    }
    else
    {
        current_countdown_text_ = QString::number(countdown_counter_);
    }
    Q_EMIT countdownTextChanged();
}

void CollageCaptureSession::setPreviewVisible(bool visible)
{
    if (preview_visible_ != visible)
    {
        preview_visible_ = visible;
        Q_EMIT previewVisibleChanged();
    }
}

void CollageCaptureSession::imageCaptured(const QImage &captured_image)
{}

void CollageCaptureSession::imageSaved(const std::filesystem::path &captured_image_path)
{
    try
    {
        auto &&image_element = context_.settings().image_elements.at(current_capture_);
        context_.renderer().setSourceOfPhoto(image_element, captured_image_path);
        current_capture_++;
        startCountdownOrFinish();
    }
    catch (const std::out_of_range &ex)
    {
        LOG_BACKTRACE(collage_capture_session,
                      "Tried to add an image which wasn't part of the collage. current_capture={}. Error: {}",
                      current_capture_,
                      ex.what());
    }
}

// void CollageCaptureSession::imageCaptured(const std::filesystem::path &captured_image_path)
//{
//     if (current_capture_ >= settings_.image_elements.size())
//     {
//         LOG_WARNING(collageCaptureSession,
//                     "Can't add image {} to collage. Maximum images already reached.",
//                     captured_image_path.string());
//         return;
//     }
//     const auto &element = settings_.image_elements.at(current_capture_);
//     renderer_.setSourceOfPhoto(element, captured_image_path);
//     current_capture_++;
//
//     if (current_capture_ >= settings_.image_elements.size())
//     {
//         Q_EMIT finished();
//     }
// }
void CollageCaptureSession::startCountdownOrFinish()
{
    if (current_capture_ == context_.settings().image_elements.size())
    {
        Q_EMIT finished();
    }
    else
    {
        countdown_timer_.start();
    }
}

void CollageCaptureSession::triggerCapture()
{
    countdown_timer_.start();
}

bool CollageCaptureSession::isPreviewVisible() const
{
    return preview_visible_;
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

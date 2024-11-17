#include "CollageCaptureSession.hpp"
#include <Pbox/Logger.hpp>
#include "CollageRenderer.hpp"

DEFINE_LOGGER(collageCaptureSession);

namespace Pbox
{
CollageCaptureSession::CollageCaptureSession(CollageSettings settings, CollageRenderer &renderer)
    : settings_{std::move(settings)}
    , renderer_{renderer}
{}

CollageCaptureSession::~CollageCaptureSession() = default;

void CollageCaptureSession::imageCaptured(const std::filesystem::path &captured_image_path)
{
    if (current_capture_ >= settings_.image_elements.size())
    {
        LOG_WARNING(collageCaptureSession,
                    "Can't add image {} to collage. Maximum images already reached.",
                    captured_image_path.string());
        return;
    }
    const auto &element = settings_.image_elements.at(current_capture_);
    renderer_.setSourceOfPhoto(element, captured_image_path);
    current_capture_++;

    if (current_capture_ >= settings_.image_elements.size())
    {
        Q_EMIT finished();
    }
}
} // namespace Pbox

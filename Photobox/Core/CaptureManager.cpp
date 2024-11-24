#include "CaptureManager.hpp"
#include "CollageCaptureSession.hpp"
namespace Pbox
{
CaptureManager::CaptureManager(ICamera &camera, CollageContext &collage_context)
    : camera_{camera}
    , session_{std::make_unique<CollageCaptureSession>(collage_context)}
{}

void CaptureManager::triggerButtonPressed()
{
    if (session_->getStatus() == ICaptureSession::Status::Idle)
    {
        session_->triggerCapture();
    }
}

Pbox::ICaptureSession *CaptureManager::getSession()
{
    return session_.get();
}

ICamera *CaptureManager::getCamera()
{
    return std::addressof(camera_);
}
} // namespace Pbox

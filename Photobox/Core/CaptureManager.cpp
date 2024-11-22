#include "CaptureManager.hpp"
#include "CollageCaptureSession.hpp"
namespace Pbox
{
CaptureManager::CaptureManager(CollageContext &collage_context)
    : session_{std::make_unique<CollageCaptureSession>(collage_context)}
{}

void CaptureManager::beginCollageCaptureSession()
{}

Pbox::ICaptureSession *CaptureManager::getSession()
{
    return session_.get();
}
} // namespace Pbox

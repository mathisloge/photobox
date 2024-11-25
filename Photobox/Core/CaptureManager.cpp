#include "CaptureManager.hpp"
#include <ICamera.hpp>
#include <Pbox/CleanupAsyncScope.hpp>
#include <Pbox/Logger.hpp>
#include <Scheduler.hpp>
#include "CollageCaptureSession.hpp"
#include "ImageProvider.hpp"
#include "ImageStorage.hpp"

DEFINE_LOGGER(capture_manager);

namespace Pbox
{
CaptureManager::CaptureManager(Scheduler &scheduler,
                               ImageStorage &image_storage,
                               ICamera &camera,
                               CollageContext &collage_context)
    : scheduler_{scheduler}
    , image_storage_{image_storage}
    , camera_{camera}
    , session_{std::make_unique<CollageCaptureSession>(collage_context)}
{
    connect(&camera_, &ICamera::imageCaptured, this, [this](auto &&image) {
        //! important: first emit the signal so that all image providers have it saved, and only then set the image to
        //! the session. otherwise qml will not reevalute the image and the providers don't have a signal to schedule a
        //! cache invalidation
        const auto image_id = image_ids_++;
        Q_EMIT imageCaptured(image, image_id);
        session_->imageCaptured(image, image_id);
        async_scope_.spawn(
            stdexec::schedule(scheduler_.getWorkScheduler()) |
            stdexec::then([this, image = image]() { return image_storage_.saveImage(image); }) |
            stdexec::continues_on(scheduler_.getQtEventLoopScheduler()) |
            stdexec::then([this](auto &&saved_image_path) { session_->imageSaved(saved_image_path); }) |
            stdexec::upon_error([](auto &&ex_ptr) { LOG_ERROR(capture_manager, "Error while saving image"); }));
    });
    connect(session_.get(), &ICaptureSession::requestedImageCapture, &camera_, &ICamera::requestCapturePhoto);
    connect(session_.get(), &ICaptureSession::finished, this, &CaptureManager::resetImages);
}

CaptureManager::~CaptureManager()
{
    cleanup_async_scope(async_scope_);
}

void CaptureManager::triggerButtonPressed()
{
    if (session_->getStatus() == ICaptureSession::Status::Idle)
    {
        session_->triggerCapture();
    }
}

ImageProvider *CaptureManager::createImageProvider()
{
    auto *image_provider = new ImageProvider();
    connect(this, &CaptureManager::imageCaptured, image_provider, &ImageProvider::addImage);
    connect(this, &CaptureManager::resetImages, image_provider, &ImageProvider::resetCache);
    return image_provider;
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

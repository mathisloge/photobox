// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CaptureManager.hpp"
#include <ICamera.hpp>
#include <Pbox/CleanupAsyncScope.hpp>
#include <Pbox/Logger.hpp>
#include <Scheduler.hpp>
#include "CameraLed.hpp"
#include "IdleCaptureSession.hpp"
#include "ImageProvider.hpp"
#include "ImageStorage.hpp"
#include "RemoteTrigger.hpp"

DEFINE_LOGGER(capture_manager);

namespace Pbox
{
CaptureManager::CaptureManager(Scheduler &scheduler,
                               ImageStorage &image_storage,
                               ICamera &camera,
                               RemoteTrigger &remote_trigger,
                               CameraLed &camera_led,
                               CaptureSessionFactoryFnc collage_session_factory)
    : scheduler_{scheduler}
    , image_storage_{image_storage}
    , camera_{camera}
    , remote_trigger_{remote_trigger}
    , camera_led_{camera_led}
    , session_{std::make_unique<IdleCaptureSession>()}
    , collage_session_factory_{std::move(collage_session_factory)}
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
    connect(&remote_trigger_, &RemoteTrigger::triggered, this, &CaptureManager::triggerButtonPressed);
    switchToSession(std::make_unique<IdleCaptureSession>());
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

void CaptureManager::sessionFinished()
{
    LOG_DEBUG(capture_manager, "Session '{}' finished", session_->name());
    Q_EMIT resetImages();
    if (session_->name() == IdleCaptureSession::kName)
    {
        switchToSession(collage_session_factory_());
        session_->triggerCapture();
    }
    else
    {
        switchToSession(std::make_unique<IdleCaptureSession>());
    }
}

void CaptureManager::switchToSession(CaptureSessionPtr &&new_session)
{
    const auto old_session_name = session_ != nullptr ? session_->name() : "unknown";
    session_ = std::move(new_session);
    connect(session_.get(), &ICaptureSession::requestedImageCapture, &camera_, &ICamera::requestCapturePhoto);
    connect(session_.get(), &ICaptureSession::finished, this, &CaptureManager::sessionFinished);
    connect(session_.get(), &ICaptureSession::statusChanged, this, &CaptureManager::handleSessionStatusChange);
    Q_EMIT sessionChanged();
    LOG_INFO(capture_manager, "Switched session from '{}' to '{}'", old_session_name, session_->name());
}

void CaptureManager::handleSessionStatusChange()
{
    const auto status = session_->getStatus();

    switch (status)
    {
    case ICaptureSession::Status::Idle:
        remote_trigger_.playEffect(RemoteTrigger::Effect::Idle);
        break;
    case ICaptureSession::Status::Capturing:
        remote_trigger_.playEffect(RemoteTrigger::Effect::Countdown);
        break;
    case ICaptureSession::Status::Busy:
        break;
    }
}

void CaptureManager::handleSessionCaptureStatusChange()
{
    const auto status = session_->getCaptureStatus();
    switch (status)
    {
    case ICaptureSession::CaptureStatus::Idle:
        camera_led_.turnOff();
        break;
    case ICaptureSession::CaptureStatus::BeforeCapture:
        camera_led_.playEffect(CameraLed::Effect::Pulsate);
        break;
    case ICaptureSession::CaptureStatus::WaitForCapture:
        camera_led_.playEffect(CameraLed::Effect::Capture);
        break;
    }
}
} // namespace Pbox

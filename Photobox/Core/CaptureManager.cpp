// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
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

DEFINE_LOGGER(capture_manager);

namespace Pbox
{
CaptureManager::CaptureManager(Scheduler &scheduler,
                               ImageStorage &image_storage,
                               ICamera &camera,
                               Instance<TriggerManager> trigger_manager,
                               Instance<CameraLed> camera_led,
                               Instance<CaptureSessionManager> capture_session_manager)
    : scheduler_{scheduler}
    , image_storage_{image_storage}
    , camera_{camera}
    , trigger_manager_{trigger_manager}
    , camera_led_{std::move(camera_led)}
    , session_{make_unique_object_ptr_as<ICaptureSession, IdleCaptureSession>()}
    , capture_session_manager_{std::move(capture_session_manager)}
{
    connect(&camera_, &ICamera::imageCaptured, this, [this](auto &&image) {
        //! important: first emit the signal so that all image providers have it saved, and only then set the image to
        //! the session. otherwise qml will not reevaluate the image and the providers don't have a signal to schedule a
        //! cache invalidation
        const auto image_id = image_ids_++;
        Q_EMIT imageCaptured(image, image_id);
        session_->imageCaptured(image, image_id);
        async_scope_.spawn(stdexec::schedule(scheduler_.getWorkScheduler()) |
                           stdexec::then([this, image = image]() { return image_storage_.saveImage(image); }) |
                           stdexec::continues_on(scheduler_.getQtEventLoopScheduler()) |
                           stdexec::then([this](auto &&saved_image_path) { session_->imageSaved(saved_image_path); }) |
                           stdexec::upon_error([](auto &&ex_ptr) {
                               try
                               {
                                   std::rethrow_exception(ex_ptr);
                               }
                               catch (const std::exception &ex)
                               {
                                   LOG_ERROR(logger_capture_manager(), "Error while saving image. {}", ex.what());
                               }
                           }));
    });
    connect(trigger_manager_.get(), &TriggerManager::triggerFired, this, [this](const TriggerId &trigger_id) {
        triggerButtonPressed(QString::fromStdString(trigger_id));
    });
    switchToSession(make_unique_object_ptr_as<ICaptureSession, IdleCaptureSession>());
}

CaptureManager::~CaptureManager()
{
    cleanup_async_scope(async_scope_);
}

void CaptureManager::triggerButtonPressed(const QString &trigger_id)
{
    if (session_->getStatus() == ICaptureSession::Status::Idle)
    {
        const auto trigger = trigger_id.toStdString();
        LOG_DEBUG(logger_capture_manager(), "Got trigger {}", trigger);
        switchToSession(capture_session_manager_->createFromTrigger(trigger));
        session_->triggerCapture();
    }
}

ImageProvider *CaptureManager::createImageProvider()
{
    auto *image_provider = new ImageProvider(); // NOLINT(cppcoreguidelines-owning-memory)
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
    LOG_DEBUG(logger_capture_manager(), "Session '{}' finished", session_->name());
    Q_EMIT resetImages();
    if (session_->name() != IdleCaptureSession::kName)
    {
        switchToSession(capture_session_manager_->createIdleSession());
    }
}

void CaptureManager::switchToSession(CaptureSessionPtr new_session)
{
    const auto old_session_name = session_ != nullptr ? session_->name() : "unknown";
    session_ = std::move(new_session);
    if (session_ != nullptr)
    {
        connect(session_.get(), &ICaptureSession::requestedImageCapture, &camera_, &ICamera::requestCapturePhoto);
        connect(session_.get(), &ICaptureSession::finished, this, &CaptureManager::sessionFinished);
        connect(session_.get(), &ICaptureSession::statusChanged, this, &CaptureManager::handleSessionStatusChange);
        connect(session_.get(),
                &ICaptureSession::captureStatusChanged,
                this,
                &CaptureManager::handleSessionCaptureStatusChange);
        handleSessionStatusChange();
        handleSessionCaptureStatusChange();
    }
    Q_EMIT sessionChanged();
    LOG_INFO(logger_capture_manager(),
             "Switched session from '{}' to '{}'",
             old_session_name,
             session_ != nullptr ? session_->name() : "none");
}

void CaptureManager::handleSessionStatusChange()
{
    const auto status = session_->getStatus();

    switch (status)
    {
    case ICaptureSession::Status::Idle:
        // remote_trigger_.playEffect(RemoteTrigger::Effect::Idle);
        break;
    case ICaptureSession::Status::Capturing:
        // remote_trigger_.playEffect(RemoteTrigger::Effect::Countdown);
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
        LOG_DEBUG(logger_capture_manager(), "Camera LED => off");
        camera_led_->turnOff();
        break;
    case ICaptureSession::CaptureStatus::BeforeCapture:
        LOG_DEBUG(logger_capture_manager(), "Camera LED => Pulsate");
        camera_led_->playEffect(CameraLed::Effect::Pulsate);
        break;
    case ICaptureSession::CaptureStatus::WaitForCapture:
        LOG_DEBUG(logger_capture_manager(), "Camera LED => Capture");
        camera_led_->playEffect(CameraLed::Effect::Capture);
        break;
    }
}
} // namespace Pbox

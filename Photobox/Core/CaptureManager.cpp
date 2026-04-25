// SPDX-FileCopyrightText: 2024 - 2026 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CaptureManager.hpp"
#include <ICamera.hpp>
#include <Pbox/Logger.hpp>
#include "CameraLed.hpp"
#include "CapturePipeline.hpp"
#include "CaptureSessionCoordinator.hpp"
#include "ImageProvider.hpp"
#include "SessionEffectController.hpp"
#include "TriggerController.hpp"

DEFINE_LOGGER(capture_manager);

namespace Pbox
{
CaptureManager::CaptureManager(Instance<Scheduler> scheduler,
                               Instance<ImageStorage> image_storage,
                               Instance<ICamera> camera,
                               Instance<TriggerManager> trigger_manager,
                               Instance<CameraLed> camera_led,
                               Instance<CaptureSessionManager> capture_session_manager)
    : camera_{std::move(camera)}
{
    session_coordinator_ = std::make_unique<CaptureSessionCoordinator>(camera_, capture_session_manager);

    trigger_controller_ = std::make_unique<TriggerController>(
        trigger_manager,
        capture_session_manager,
        [this]() { return session_coordinator_->getSession(); },
        [this](CaptureSessionPtr session) { session_coordinator_->switchToSession(std::move(session)); });

    session_effect_controller_ = std::make_unique<SessionEffectController>(
        trigger_manager, camera_led, [this]() -> const TriggerId & { return trigger_controller_->getTriggeredBy(); });

    capture_pipeline_ = std::make_unique<CapturePipeline>(
        scheduler, image_storage, [this]() { return session_coordinator_->getSession(); });

    connect(camera_.get(), &ICamera::imageCaptured, capture_pipeline_.get(), &CapturePipeline::onCameraImageCaptured);
    connect(capture_pipeline_.get(),
            &CapturePipeline::imageCaptured,
            this,
            [this](const QImage &image, std::uint32_t image_id) { Q_EMIT imageCaptured(image, image_id); });
    connect(session_coordinator_.get(), &CaptureSessionCoordinator::finished, this, &CaptureManager::sessionFinished);
    connect(
        session_coordinator_.get(), &CaptureSessionCoordinator::sessionChanged, this, &CaptureManager::sessionChanged);
    connect(session_coordinator_.get(), &CaptureSessionCoordinator::statusChanged, this, [this]() {
        if (auto *session = session_coordinator_->getSession())
        {
            session_effect_controller_->handleSessionStatusChanged(session->getStatus());
        }
    });
    connect(session_coordinator_.get(), &CaptureSessionCoordinator::captureStatusChanged, this, [this]() {
        if (auto *session = session_coordinator_->getSession())
        {
            session_effect_controller_->handleSessionCaptureStatusChanged(session->getCaptureStatus());
        }
    });

    session_coordinator_->initialize();
}

CaptureManager::~CaptureManager() = default;

void CaptureManager::triggerButtonPressed(const QString &trigger_id)
{
    trigger_controller_->triggerButtonPressed(trigger_id);
}

void CaptureManager::sessionButtonPressed(const QString &session_id)
{
    trigger_controller_->sessionButtonPressed(session_id);
}

ImageProvider *CaptureManager::createImageProvider() const
{
    auto *image_provider = new ImageProvider(); // NOLINT(cppcoreguidelines-owning-memory)
    connect(this, &CaptureManager::imageCaptured, image_provider, &ImageProvider::addImage);
    connect(this, &CaptureManager::resetImages, image_provider, &ImageProvider::resetCache);
    return image_provider;
}

Pbox::ICaptureSession *CaptureManager::getSession()
{
    return session_coordinator_->getSession();
}

ICamera *CaptureManager::getCamera()
{
    return camera_.get();
}

void CaptureManager::sessionFinished()
{
    Q_EMIT resetImages();
}
} // namespace Pbox

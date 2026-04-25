// SPDX-FileCopyrightText: 2026 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SessionEffectController.hpp"

namespace Pbox
{
SessionEffectController::SessionEffectController(Instance<TriggerManager> trigger_manager,
                                                 Instance<CameraLed> camera_led,
                                                 std::function<const TriggerId &()> triggered_by_callback)
    : trigger_manager_{std::move(trigger_manager)}
    , camera_led_{std::move(camera_led)}
    , triggered_by_callback_{std::move(triggered_by_callback)}
{}

void SessionEffectController::handleSessionStatusChanged(ICaptureSession::Status status)
{
    const auto &triggered_by = triggered_by_callback_();
    if (triggered_by.empty())
    {
        return;
    }

    switch (status)
    {
    case ICaptureSession::Status::Idle:
        trigger_manager_->updateTriggerEffect(triggered_by, RemoteTrigger::Effect::Idle);
        break;
    case ICaptureSession::Status::Capturing:
        trigger_manager_->updateTriggerEffect(triggered_by, RemoteTrigger::Effect::Countdown);
        break;
    case ICaptureSession::Status::Busy:
        break;
    }
}

void SessionEffectController::handleSessionCaptureStatusChanged(ICaptureSession::CaptureStatus capture_status)
{
    switch (capture_status)
    {
    case ICaptureSession::CaptureStatus::Idle:
        camera_led_->turnOff();
        break;
    case ICaptureSession::CaptureStatus::BeforeCapture:
        camera_led_->playEffect(CameraLed::Effect::Pulsate);
        break;
    case ICaptureSession::CaptureStatus::WaitForCapture:
        camera_led_->playEffect(CameraLed::Effect::Capture);
        break;
    }
}
} // namespace Pbox

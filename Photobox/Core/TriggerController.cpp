// SPDX-FileCopyrightText: 2026 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TriggerController.hpp"
#include <Pbox/Logger.hpp>

DEFINE_LOGGER(trigger_controller);

namespace Pbox
{
TriggerController::TriggerController(Instance<TriggerManager> trigger_manager,
                                     Instance<CaptureSessionManager> capture_session_manager,
                                     std::function<ICaptureSession *()> get_current_session_callback,
                                     std::function<void(CaptureSessionPtr)> switch_session_callback,
                                     QObject *parent)
    : QObject(parent)
    , get_current_session_callback_{std::move(get_current_session_callback)}
    , switch_session_callback_{std::move(switch_session_callback)}
    , trigger_manager_{std::move(trigger_manager)}
    , capture_session_manager_{std::move(capture_session_manager)}
{
    connect(trigger_manager_.get(), &TriggerManager::triggerFired, this, &TriggerController::handleTriggerFired);
}

bool TriggerController::canSwitchSession() const
{
    if (auto *session = get_current_session_callback_())
    {
        return session->getStatus() == ICaptureSession::Status::Idle;
    }
    return false;
}

void TriggerController::triggerButtonPressed(const QString &trigger_id)
{
    if (not canSwitchSession())
    {
        return;
    }

    const auto trigger = trigger_id.toStdString();
    triggered_by_ = trigger;
    LOG_DEBUG(logger_trigger_controller(), "Got trigger {}", trigger);
    switch_session_callback_(capture_session_manager_->createFromTrigger(trigger));

    if (auto *session = get_current_session_callback_())
    {
        session->triggerCapture();
    }
}

void TriggerController::sessionButtonPressed(const QString &session_id)
{
    if (not canSwitchSession())
    {
        return;
    }

    triggered_by_.clear();
    LOG_DEBUG(logger_trigger_controller(), "Got session display trigger {}", session_id.toStdString());
    switch_session_callback_(capture_session_manager_->createFromSessionId(session_id.toStdString()));

    if (auto *session = get_current_session_callback_())
    {
        session->triggerCapture();
    }
}

void TriggerController::handleTriggerFired(const TriggerId &trigger_id)
{
    triggerButtonPressed(QString::fromStdString(trigger_id));
}

const TriggerId &TriggerController::getTriggeredBy() const
{
    return triggered_by_;
}
} // namespace Pbox

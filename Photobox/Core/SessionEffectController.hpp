// SPDX-FileCopyrightText: 2026 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <functional>
#include <Pbox/Instance.hpp>
#include "CameraLed.hpp"
#include "ICaptureSession.hpp"
#include "TriggerId.hpp"
#include "TriggerManager.hpp"

namespace Pbox
{
/**
 * @brief Maps capture session state changes to visual effects.
 *
 * SessionEffectController applies LED effects and updates remote trigger status
 * based on the active session lifecycle.
 */
class SessionEffectController
{
  public:
    /**
     * @brief Constructs the session effect controller.
     * @param trigger_manager Trigger manager used for effect updates.
     * @param camera_led Camera LED controller used for visual feedback.
     * @param triggered_by_callback Callback returning the active trigger ID.
     */
    SessionEffectController(Instance<TriggerManager> trigger_manager,
                            Instance<CameraLed> camera_led,
                            std::function<const TriggerId &()> triggered_by_callback);
    PBOX_DISABLE_COPY_MOVE(SessionEffectController);
    ~SessionEffectController() = default;

    /**
     * @brief Handles a session status update.
     * @param status Current session status.
     */
    void handleSessionStatusChanged(ICaptureSession::Status status);
    /**
     * @brief Handles a capture status update.
     * @param capture_status Current capture status.
     */
    void handleSessionCaptureStatusChanged(ICaptureSession::CaptureStatus capture_status);

  private:
    Instance<TriggerManager> trigger_manager_;                 /**< Trigger manager for remote effects. */
    Instance<CameraLed> camera_led_;                           /**< Camera LED controller. */
    std::function<const TriggerId &()> triggered_by_callback_; /**< Returns the active trigger ID. */
};
} // namespace Pbox

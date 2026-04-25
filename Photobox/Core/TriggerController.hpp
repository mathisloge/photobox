// SPDX-FileCopyrightText: 2026 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <functional>
#include <Pbox/Instance.hpp>
#include "CaptureSessionManager.hpp"
#include "ICaptureSession.hpp"
#include "TriggerId.hpp"
#include "TriggerManager.hpp"

namespace Pbox
{
/**
 * @brief Handles trigger and session button events.
 *
 * TriggerController decides when a session may start and requests the
 * appropriate capture session from the session manager.
 */
class TriggerController : public QObject
{
    Q_OBJECT

  public:
    /**
     * @brief Constructs the trigger controller.
     * @param trigger_manager Trigger manager handling remote input.
     * @param capture_session_manager Manager to create sessions from IDs or triggers.
     * @param get_current_session_callback Callback to inspect the active session.
     * @param switch_session_callback Callback to activate a new session.
     * @param parent QObject parent.
     */
    TriggerController(Instance<TriggerManager> trigger_manager,
                      Instance<CaptureSessionManager> capture_session_manager,
                      std::function<ICaptureSession *()> get_current_session_callback,
                      std::function<void(CaptureSessionPtr)> switch_session_callback,
                      QObject *parent = nullptr);
    PBOX_DISABLE_COPY_MOVE(TriggerController);
    ~TriggerController() override = default;

    /**
     * @brief Invoked when a remote trigger button is pressed.
     * @param trigger_id Trigger identifier.
     */
    void triggerButtonPressed(const QString &trigger_id);
    /**
     * @brief Invoked when a session button is pressed.
     * @param session_id Session identifier.
     */
    void sessionButtonPressed(const QString &session_id);
    /**
     * @brief Gets the ID of the last trigger that initiated the current session.
     * @return Trigger identifier or empty if none.
     */
    const TriggerId &getTriggeredBy() const;

  private Q_SLOTS:
    /**
     * @brief Slot called when a remote trigger fires.
     * @param trigger_id Trigger identifier.
     */
    void handleTriggerFired(const TriggerId &trigger_id);

  private:
    bool canSwitchSession() const;

    std::function<ICaptureSession *()> get_current_session_callback_; /**< Active session query callback. */
    std::function<void(CaptureSessionPtr)> switch_session_callback_;  /**< Session activation callback. */
    Instance<TriggerManager> trigger_manager_;                        /**< Remote trigger manager. */
    Instance<CaptureSessionManager> capture_session_manager_;         /**< Session factory provider. */
    TriggerId triggered_by_;                                          /**< Currently active trigger ID. */
};
} // namespace Pbox

// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include "Pbox/Instance.hpp"
#include "RemoteTrigger.hpp"
#include "TriggerId.hpp"

namespace Pbox
{
class SystemStatusManager;

/**
 * @brief Manager for remote triggers and their effects.
 *
 * This class handles the registration of remote triggers, connects their signals,
 * and coordinates effect changes across all triggers.
 */
class TriggerManager : public QObject
{
    Q_OBJECT

  public:
    PBOX_DISABLE_COPY_MOVE(TriggerManager);
    /**
     * @brief Constructs a TriggerManager.
     * @param system_status_manager The system status manager.
     */
    explicit TriggerManager(Instance<SystemStatusManager> system_status_manager);
    ~TriggerManager() override = default;
    /**
     * @brief Registers a remote trigger.
     * @param triggerId The ID of the trigger.
     * @param trigger The trigger to register.
     */
    void registerTrigger(TriggerId triggerId, std::unique_ptr<RemoteTrigger> trigger);
    /**
     * @brief Updates the effect of a trigger.
     * @param triggerId The ID of the trigger.
     * @param effect The new effect.
     */
    void updateTriggerEffect(TriggerId triggerId, RemoteTrigger::Effect effect);

  Q_SIGNALS:
    /**
     * @brief Emitted when a trigger is fired.
     * @param triggerId The ID of the trigger that fired.
     */
    void triggerFired(TriggerId triggerId);

  private:
    Instance<SystemStatusManager> system_status_manager_;                           /**< System status manager. */
    std::unordered_map<TriggerId, std::unique_ptr<RemoteTrigger>> remote_triggers_; /**< Registered triggers. */
};
} // namespace Pbox

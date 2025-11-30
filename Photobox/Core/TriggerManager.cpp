// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TriggerManager.hpp"
#include <Pbox/Logger.hpp>
#include "SystemStatusManager.hpp"

DEFINE_LOGGER(trigger_manager);

namespace Pbox
{
TriggerManager::TriggerManager(Instance<SystemStatusManager> system_status_manager)
    : system_status_manager_{std::move(system_status_manager)}
{}

void TriggerManager::registerTrigger(TriggerId triggerId, std::unique_ptr<RemoteTrigger> trigger)
{
    LOG_DEBUG(logger_trigger_manager(), "Register remote trigger '{}'", triggerId);
    const auto [entry, emplaced] = remote_triggers_.emplace(triggerId, std::move(trigger));
    if (not emplaced)
    {
        LOG_ERROR(logger_trigger_manager(), "Could not register trigger '{}', it already exists.", entry->first);
    }
    else
    {
        connect(entry->second.get(), &RemoteTrigger::triggered, this, [this, triggerId]() {
            LOG_INFO(logger_trigger_manager(), "Trigger '{}' fired.", triggerId);
            Q_EMIT triggerFired(triggerId);
        });
        entry->second->playEffect(RemoteTrigger::Effect::Idle);
        system_status_manager_->registerClient(entry->second->systemStatusClient());
    }
}

void TriggerManager::updateTriggerEffect(TriggerId triggerId, RemoteTrigger::Effect effect)
{
    const QMetaEnum effect_meta_enum = QMetaEnum::fromType<RemoteTrigger::Effect>();
    auto &&effect_str = effect_meta_enum.valueToKey(static_cast<int>(effect));

    auto it = remote_triggers_.find(triggerId);
    if (it == remote_triggers_.end())
    {
        LOG_ERROR(
            logger_trigger_manager(), "Could not find trigger '{}' to update effect to '{}'", triggerId, effect_str);
        return;
    }
    LOG_DEBUG(logger_trigger_manager(), "Update effect of trigger '{}' to effect '{}'", triggerId, effect_str);
    it->second->playEffect(effect);
}

} // namespace Pbox

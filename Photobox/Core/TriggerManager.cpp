// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TriggerManager.hpp"
#include <Pbox/Logger.hpp>

DEFINE_LOGGER(log_trigger_manager);

namespace Pbox
{
void TriggerManager::registerTrigger(TriggerId triggerId, std::unique_ptr<RemoteTrigger> trigger)
{
    LOG_DEBUG(log_trigger_manager, "Register remote trigger '{}'", triggerId);
    const auto [entry, emplaced] = remote_triggers_.emplace(std::move(triggerId), std::move(trigger));
    if (not emplaced)
    {
        LOG_ERROR(log_trigger_manager, "Could not register trigger '{}', it already exists.", entry->first);
    }
}
} // namespace Pbox

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

class TriggerManager : public QObject
{
    Q_OBJECT

  public:
    PBOX_DISABLE_COPY_MOVE(TriggerManager);
    explicit TriggerManager(Instance<SystemStatusManager> system_status_manager);
    ~TriggerManager() override = default;
    void registerTrigger(TriggerId triggerId, std::unique_ptr<RemoteTrigger> trigger);
    void updateTriggerEffect(TriggerId triggerId, RemoteTrigger::Effect effect);

  Q_SIGNALS:
    void triggerFired(TriggerId triggerId);

  private:
    Instance<SystemStatusManager> system_status_manager_;
    std::unordered_map<TriggerId, std::unique_ptr<RemoteTrigger>> remote_triggers_;
};
} // namespace Pbox

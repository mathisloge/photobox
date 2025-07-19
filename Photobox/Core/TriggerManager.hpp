// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include "RemoteTrigger.hpp"

namespace Pbox
{
using TriggerId = std::string;
class TriggerManager : public QObject
{
    Q_OBJECT

  public:
    void registerTrigger(TriggerId triggerId, std::unique_ptr<RemoteTrigger> trigger);

  Q_SIGNALS:
    void triggerFired(TriggerId triggerId);

  private:
    std::unordered_map<TriggerId, std::unique_ptr<RemoteTrigger>> remote_triggers_;
};
} // namespace Pbox

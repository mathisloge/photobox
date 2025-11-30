// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "RemoteTrigger.hpp"
namespace Pbox
{
RemoteTrigger::RemoteTrigger(QString name)
    : system_status_client_{std::move(name), false}
{}

RemoteTrigger::~RemoteTrigger() = default;

const SystemStatusClient &RemoteTrigger::systemStatusClient() const
{
    return system_status_client_;
}
} // namespace Pbox

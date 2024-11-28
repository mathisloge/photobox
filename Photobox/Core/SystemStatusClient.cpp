// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SystemStatusClient.hpp"

namespace Pbox
{
SystemStatusClient::SystemStatusClient(QString name, bool required)
    : name_{std::move(name)}
    , required_{std::move(required)}
{}

SystemStatusClient::~SystemStatusClient() = default;

void SystemStatusClient::setSystemStatus(SystemStatusCode::Code code)
{
    if (code_ != code)
    {
        code_ = code;
        Q_EMIT systemStatusChanged();
    }
}

SystemStatusCode::Code SystemStatusClient::systemStatus() const
{
    return code_;
}

const QString &SystemStatusClient::name() const
{
    return name_;
}

bool SystemStatusClient::isRequired() const
{
    return required_;
}

} // namespace Pbox

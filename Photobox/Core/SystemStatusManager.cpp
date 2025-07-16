// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SystemStatusManager.hpp"

namespace Pbox
{
void SystemStatusManager::registerClient(const Pbox::SystemStatusClient *client)
{
    model_.addClient(client);
    clients_.emplace_back(client);
    connect(client, &SystemStatusClient::systemStatusChanged, this, &SystemStatusManager::evaluateSystemStatus);
    evaluateSystemStatus();
}

void SystemStatusManager::evaluateSystemStatus()
{
    bool required_ready{true};
    bool all_ready{true};
    for (auto &&client : clients_)
    {
        if (client->systemStatus() != SystemStatusCode::Code::Ok)
        {
            all_ready = false;
            if (client->isRequired())
            {
                required_ready = false;
                break;
            }
        }
    }

    const Status new_status = [&]() {
        if (all_ready)
        {
            return Status::AllClientsInitializied;
        }
        if (required_ready)
        {
            return Status::RequiredClientsInitilized;
        }
        return Status::Initializing;
    }();

    if (new_status != status_)
    {
        status_ = new_status;
        Q_EMIT statusChanged();
    }
}

SystemStatusModel *SystemStatusManager::getModel()
{
    return std::addressof(model_);
}

SystemStatusManager::Status SystemStatusManager::getStatus() const
{
    return status_;
}
} // namespace Pbox

// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathis.opensource@tuta.io>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SystemStatusModel.hpp"
#include <Pbox/Logger.hpp>

DEFINE_LOGGER(log_system_status_model);

namespace Pbox
{
int SystemStatusModel::rowCount(const QModelIndex & /*parent*/) const
{
    return static_cast<int>(clients_.size());
}

QVariant SystemStatusModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= clients_.size())
    {
        return {};
    }
    const auto &client = clients_.at(index.row());
    if (client.isNull())
    {
        LOG_WARNING(log_system_status_model, "Got dangling system client.");
        return {};
    }
    switch (Role{role})
    {
    case Role::Title:
        return client->name();
    case Role::Status:
        return QVariant::fromValue(client->systemStatus());
    }
    return {};
}

QHash<int, QByteArray> SystemStatusModel::roleNames() const
{
    static const auto kRoles = QHash<int, QByteArray>{
        {std::to_underlying(Role::Title), "title"},
        {std::to_underlying(Role::Status), "status"},
    };
    return kRoles;
}

void SystemStatusModel::addClient(const SystemStatusClient *client)
{
    const int new_index = static_cast<int>(clients_.size());
    beginInsertRows(QModelIndex{}, new_index, new_index);
    clients_.emplace_back(client);
    connect(client, &SystemStatusClient::systemStatusChanged, this, [this, row = new_index]() {
        Q_EMIT dataChanged(index(row), index(row), {std::to_underlying(Role::Status)});
    });
    endInsertRows();
}
} // namespace Pbox

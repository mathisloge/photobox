#include "SystemStatusModel.hpp"

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
    switch (Role{role})
    {
    case Role::Title:
        return client.name();
    case Role::Status:
        return QVariant::fromValue(client.systemStatus());
    }
    return {};
}

QHash<int, QByteArray> SystemStatusModel::roleNames() const
{
    static const auto kRoles = QHash<int, QByteArray>{
        {std::underlying_type_t<Role>(Role::Title), "title"},
        {std::underlying_type_t<Role>(Role::Status), "status"},
    };
    return kRoles;
}

void SystemStatusModel::registerClient(SystemStatusClient &&client)
{
    beginInsertRows(QModelIndex{}, clients_.size(), clients_.size());
    clients_.emplace_back(std::move(client));
    endInsertRows();
}
} // namespace Pbox

#include "SystemStatusModel.hpp"

namespace Pbox
{
int SystemStatusModel::rowCount(const QModelIndex &parent) const
{}

QVariant SystemStatusModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() /*|| index.row() >= image_sources_.size()*/)
    {
        return {};
    }
}

QHash<int, QByteArray> SystemStatusModel::roleNames() const
{
    static const auto kRoles = QHash<int, QByteArray>{
        {std::underlying_type_t<Role>(Role::Title), "title"},
        {std::underlying_type_t<Role>(Role::Status), "status"},
    };
    return kRoles;
}
} // namespace Pbox

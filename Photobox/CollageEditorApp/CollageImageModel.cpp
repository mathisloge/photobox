// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathis.opensource@tuta.io>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CollageImageModel.hpp"

namespace Pbox
{
CollageImageModel::CollageImageModel(CollageRenderer &renderer)
    : renderer_{renderer}
{}

int CollageImageModel::rowCount(const QModelIndex & /*parent*/) const
{
    return static_cast<int>(renderer_.registeredImages().size());
}

QVariant CollageImageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= renderer_.registeredImages().size())
    {
        return {};
    }
    const auto key = key_map_.at(index.row());
    const auto &element = renderer_.registeredImages().at(key);
    switch (Roles{role})
    {
    case Roles::ElementId:
        return QVariant::fromValue(QString::fromStdString(key));
    }
    return {};
}

QHash<int, QByteArray> CollageImageModel::roleNames() const
{
    static const auto kRoles = QHash<int, QByteArray>{
        {std::underlying_type_t<Roles>(Roles::ElementId), "elementId"},
    };

    return kRoles;
}

bool CollageImageModel::appendElement(const QString &element_id)
{
    const auto element_str = element_id.toStdString();
    const auto it = renderer_.registeredImages().find(element_str);
    if (it != renderer_.registeredImages().cend())
    {
        return false;
    }
    renderer_.addPhotoElement(element_str);

    beginInsertRows({}, key_map_.size(), key_map_.size());
    key_map_.emplace_back(element_str);
    endInsertRows();
    return true;
}

void CollageImageModel::removeElement(const QString &element_id)
{
    const auto element_str = element_id.toStdString();
    const auto it = std::find(key_map_.begin(), key_map_.end(), element_str);
    if (it == key_map_.end())
    {
        return;
    }
    const auto pos = std::distance(key_map_.begin(), it);

    beginRemoveRows({}, pos, pos);
    key_map_.erase(it);
    renderer_.removePhotoElement(element_str);
    endRemoveRows();
}
} // namespace Pbox

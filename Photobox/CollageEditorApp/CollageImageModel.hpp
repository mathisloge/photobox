// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QAbstractListModel>
#include <QtQmlIntegration>
#include "CollageRenderer.hpp"

namespace Pbox
{
class CollageImageModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ANONYMOUS
  public:
    enum class Roles
    {
        ElementId = Qt::DisplayRole + 1
    };
    explicit CollageImageModel(CollageRenderer &renderer);

    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE bool appendElement(const QString &element_id);
    Q_INVOKABLE void removeElement(const QString &element_id);

  private:
    CollageRenderer &renderer_;
    std::vector<std::string> key_map_;
};
} // namespace Pbox
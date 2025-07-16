// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QAbstractListModel>
#include <QtQmlIntegration>
#include <vector>
#include <Pbox/DisableCopyMove.hpp>
#include "SystemStatusClient.hpp"

namespace Pbox
{
class SystemStatusModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Will be provided by C++")
  public:
    enum class Role
    {
        Title = Qt::DisplayRole + 1,
        Status,
    };

  public:
    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addClient(const SystemStatusClient *client);

  private:
    std::vector<QPointer<const SystemStatusClient>> clients_;
};
} // namespace Pbox

// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QAbstractListModel>
#include <QPointer>
#include <vector>
#include <Pbox/DisableCopyMove.hpp>
#include <QtQmlIntegration/qqmlintegration.h>
#include "SystemStatusClient.hpp"

namespace Pbox
{
/**
 * @brief A Qt model for displaying system status clients in a list view.
 *
 * This model manages a list of SystemStatusClient objects and provides
 * data for QML views with roles for title and status.
 */
class SystemStatusModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Will be provided by C++")
  public:
    /**
     * @brief Roles for the model data.
     */
    enum class Role
    {
        Title = Qt::DisplayRole + 1, // The title of the system status client.
        Status,                      // The current status of the client.
    };

  public:
    /**
     * @brief Returns the number of rows in the model.
     * @param parent The parent model index (unused).
     * @return The number of clients.
     */
    int rowCount(const QModelIndex &parent = QModelIndex{}) const override;
    /**
     * @brief Returns the data for the given index and role.
     * @param index The model index.
     * @param role The role to query.
     * @return The data for the role.
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    /**
     * @brief Returns the role names for QML.
     * @return A hash of role names.
     */
    QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief Adds a system status client to the model.
     * @param client The client to add.
     */
    void addClient(const SystemStatusClient &client);
    /**
     * @brief Removes a system status client from the model.
     * @param client The client to remove.
     */
    void removeClient(const SystemStatusClient &client);

  private:
    std::vector<QPointer<const SystemStatusClient>> clients_;
};
} // namespace Pbox

// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QAbstractListModel>
#include <QtQmlIntegration/qqmlintegration.h>
#include "CollageRenderer.hpp"

namespace Pbox
{
/**
 * @brief Model for collage images in the editor.
 *
 * This model manages a list of collage elements for display in QML.
 */
class CollageImageModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ANONYMOUS
  public:
    /**
     * @brief Roles for the model data.
     */
    enum class Roles
    {
        ElementId = Qt::DisplayRole + 1 // The ID of the collage element.
    };
    /**
     * @brief Constructs a CollageImageModel.
     * @param renderer Reference to the collage renderer.
     */
    explicit CollageImageModel(CollageRenderer &renderer);

    /**
     * @brief Returns the number of rows in the model.
     * @param parent The parent model index.
     * @return The number of elements.
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
     * @brief Appends a new element to the collage.
     * @param element_id The ID of the element to append.
     * @return True if successful.
     */
    Q_INVOKABLE bool appendElement(const QString &element_id);
    /**
     * @brief Removes an element from the collage.
     * @param element_id The ID of the element to remove.
     */
    Q_INVOKABLE void removeElement(const QString &element_id);

  private:
    CollageRenderer &renderer_;
    std::vector<std::string> key_map_;
};
} // namespace Pbox

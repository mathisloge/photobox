// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QQuickPaintedItem>
#include "CollageImageModel.hpp"
#include "CollageRenderer.hpp"
namespace Pbox
{
/**
 * @brief QML component for collage template editing.
 *
 * This class provides a QML-exposed editor for arranging photos
 * in SVG collage templates, with methods to load templates and
 * assign images to elements.
 */
class CollageImage : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(CollageImageModel *model READ getModel CONSTANT FINAL)
  public:
    using QQuickPaintedItem::QQuickPaintedItem;
    /**
     * @brief Paints the rendered collage.
     * @param painter The painter to use.
     */
    void paint(QPainter *painter) override;

    /**
     * @brief Loads an SVG template file.
     * @param file_path The path to the SVG file.
     */
    Q_INVOKABLE void loadSource(QString file_path);
    /**
     * @brief Sets the source image for a collage element.
     * @param element_id The ID of the element.
     * @param file_path The path to the image file.
     */
    Q_INVOKABLE void setSourceOfPhoto(const QString &element_id, QString file_path);
    /**
     * @brief Saves the current configuration.
     */
    Q_INVOKABLE void saveConfiguration();
    /**
     * @brief Gets the associated image model.
     * @return Pointer to the model.
     */
    CollageImageModel *getModel();

  private:
    CollageRenderer renderer_;           /**< The collage renderer. */
    CollageImageModel model_{renderer_}; /**< The image model. */
};
} // namespace Pbox

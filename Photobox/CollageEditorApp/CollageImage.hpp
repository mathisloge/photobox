// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QQuickPaintedItem>
#include "CollageImageModel.hpp"
#include "CollageRenderer.hpp"
namespace Pbox
{
class CollageImage : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(CollageImageModel *model READ getModel CONSTANT FINAL)
  public:
    using QQuickPaintedItem::QQuickPaintedItem;
    void paint(QPainter *painter) override;

    Q_INVOKABLE void loadSource(QString file_path);
    Q_INVOKABLE void setSourceOfPhoto(const QString &element_id, QString file_path);
    Q_INVOKABLE void saveConfiguration();
    CollageImageModel *getModel();

  private:
    CollageRenderer renderer_;
    CollageImageModel model_{renderer_};
};
} // namespace Pbox
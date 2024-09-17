#pragma once
#include <QQuickPaintedItem>
#include "CollageRenderer.hpp"

namespace Pbox
{
class CollageImage : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT

  public:
    using QQuickPaintedItem::QQuickPaintedItem;
    void paint(QPainter *painter) override;

    Q_INVOKABLE void loadSource(QString file_path);

  private:
    CollageRenderer renderer_;
};
} // namespace Pbox
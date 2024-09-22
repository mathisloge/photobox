#pragma once
#include <QObject>
#include <QtQmlIntegration>
namespace Pbox
{
class PrintController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
  public:
    Q_INVOKABLE void showPrintSettings();
};
} // namespace Pbox
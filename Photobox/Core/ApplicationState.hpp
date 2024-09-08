#pragma once
#include <QObject>
#include <ICamera.hpp>
#include <QtQmlIntegration>

namespace Pbox
{
class ApplicationState : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(ICamera *camera READ getCamera CONSTANT)

  public:
    ICamera *getCamera()
    {
        return camera.get();
    }

  public:
    std::shared_ptr<ICamera> camera;
};
} // namespace Pbox
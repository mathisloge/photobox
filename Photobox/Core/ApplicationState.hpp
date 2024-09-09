#pragma once
#include <QObject>
#include <QtQmlIntegration>
#include <ICamera.hpp>
#include <PhotoTriggerClient.hpp>

namespace Pbox
{
class ApplicationState : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(ICamera *camera READ getCamera CONSTANT)
    Q_PROPERTY(PhotoTriggerClient *triggerClient READ getTriggerClient CONSTANT)

  public:
    ICamera *getCamera()
    {
        return camera.get();
    }

    PhotoTriggerClient *getTriggerClient()
    {
        return trigger_client.get();
    }

  public:
    std::shared_ptr<ICamera> camera;
    std::shared_ptr<PhotoTriggerClient> trigger_client;
};
} // namespace Pbox
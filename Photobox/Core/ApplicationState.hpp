#pragma once
#include <QObject>
#include <QtQmlIntegration>
#include <ICamera.hpp>
#include <PhotoTriggerClient.hpp>
#include "CaptureController.hpp"

namespace Pbox
{
class ApplicationState : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(ICamera *camera READ getCamera CONSTANT)
    Q_PROPERTY(PhotoTriggerClient *triggerClient READ getTriggerClient CONSTANT)
    Q_PROPERTY(CaptureController *captureController READ getCaptureController CONSTANT)
  public:
    ICamera *getCamera()
    {
        return camera.get();
    }

    PhotoTriggerClient *getTriggerClient()
    {
        return trigger_client.get();
    }

    CaptureController *getCaptureController()
    {
        return capture_controller.get();
    }

  public:
    std::shared_ptr<CaptureController> capture_controller;
    std::shared_ptr<ICamera> camera;
    std::shared_ptr<PhotoTriggerClient> trigger_client;
};
} // namespace Pbox
// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathis.opensource@tuta.io>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <QtQmlIntegration>
#include <ICamera.hpp>
#include <RemoteTrigger.hpp>
#include "CameraLed.hpp"
#include "CaptureManager.hpp"
#include "SystemStatusManager.hpp"

namespace Pbox
{
class ApplicationState : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(Pbox::ICamera *camera READ getCamera CONSTANT);
    Q_PROPERTY(Pbox::RemoteTrigger *remoteTrigger READ getRemoteTrigger CONSTANT);
    Q_PROPERTY(Pbox::CameraLed *cameraLed READ getCameraLed CONSTANT);
    Q_PROPERTY(Pbox::SystemStatusManager *systemStatusManager READ getSystemStatusManager CONSTANT);
    Q_PROPERTY(Pbox::CaptureManager *captureManager READ getCaptureManager CONSTANT);

  public:
    ICamera *getCamera()
    {
        return camera.get();
    }

    RemoteTrigger *getRemoteTrigger() const
    {
        return remote_trigger;
    }

    CameraLed *getCameraLed() const
    {
        return camera_led;
    }

    SystemStatusManager *getSystemStatusManager() const
    {
        return system_status_manager;
    }

    CaptureManager *getCaptureManager() const
    {
        return capture_manager;
    }

  public:
    std::shared_ptr<ICamera> camera;
    RemoteTrigger *remote_trigger{};
    CameraLed *camera_led{};
    SystemStatusManager *system_status_manager{};
    CaptureManager *capture_manager{};
};
} // namespace Pbox

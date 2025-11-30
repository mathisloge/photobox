// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <QtQmlIntegration>
#include <ICamera.hpp>
#include <RemoteTrigger.hpp>
#include "CaptureManager.hpp"
#include "Pbox/Instance.hpp"
#include "SystemStatusManager.hpp"

namespace Pbox
{
class ApplicationState : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(Pbox::SystemStatusManager *systemStatusManager READ getSystemStatusManager CONSTANT);
    Q_PROPERTY(Pbox::CaptureManager *captureManager READ getCaptureManager CONSTANT);

  public:
    SystemStatusManager *getSystemStatusManager() const
    {
        return system_status_manager.get();
    }

    CaptureManager *getCaptureManager() const
    {
        return capture_manager.get();
    }

  public:
    Instance<SystemStatusManager> system_status_manager;
    Instance<CaptureManager> capture_manager;
};
} // namespace Pbox

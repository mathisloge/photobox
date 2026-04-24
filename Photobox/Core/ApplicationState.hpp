// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <ICamera.hpp>
#include <QtQmlIntegration/qqmlintegration.h>
#include <RemoteTrigger.hpp>
#include "CaptureManager.hpp"
#include "CaptureSessionManager.hpp"
#include "Pbox/Instance.hpp"
#include "SystemStatusManager.hpp"

namespace Pbox
{
/**
 * @brief QML singleton providing global application state managers.
 *
 * This class serves as the root container for the main application managers,
 * exposing them as QML properties for global access throughout the application.
 */
class ApplicationState : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(Pbox::SystemStatusManager *systemStatusManager READ getSystemStatusManager CONSTANT);
    Q_PROPERTY(Pbox::CaptureManager *captureManager READ getCaptureManager CONSTANT);
    Q_PROPERTY(Pbox::CaptureSessionManager *captureSessionManager READ getCaptureSessionManager CONSTANT);

  public:
    /**
     * @brief Gets the system status manager.
     * @return Pointer to the system status manager.
     */
    SystemStatusManager *getSystemStatusManager() const
    {
        return system_status_manager.get();
    }

    /**
     * @brief Gets the capture manager.
     * @return Pointer to the capture manager.
     */
    CaptureManager *getCaptureManager() const
    {
        return capture_manager.get();
    }

    /**
     * @brief Gets the capture session manager.
     * @return Pointer to the capture session manager.
     */
    CaptureSessionManager *getCaptureSessionManager() const
    {
        return capture_session_manager.get();
    }

  public:
    Instance<SystemStatusManager> system_status_manager;
    Instance<CaptureManager> capture_manager;
    Instance<CaptureSessionManager> capture_session_manager;
};
} // namespace Pbox

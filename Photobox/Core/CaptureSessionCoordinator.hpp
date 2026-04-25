// SPDX-FileCopyrightText: 2026 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QObject>
#include <Pbox/Instance.hpp>
#include <Pbox/ObjectUniquePtr.hpp>
#include "CaptureSessionManager.hpp"
#include "ICamera.hpp"
#include "ICaptureSession.hpp"
#include "IdleCaptureSession.hpp"

namespace Pbox
{
/**
 * @brief Coordinates active capture session lifecycle.
 *
 * The CaptureSessionCoordinator owns the currently active session and
 * forwards session signals to interested consumers.
 */
class CaptureSessionCoordinator : public QObject
{
    Q_OBJECT

  public:
    /**
     * @brief Constructs the session coordinator.
     * @param camera Camera used to request capture operations.
     * @param capture_session_manager Manager creating capture sessions.
     * @param parent QObject parent.
     */
    explicit CaptureSessionCoordinator(Instance<ICamera> camera,
                                       Instance<CaptureSessionManager> capture_session_manager);
    PBOX_DISABLE_COPY_MOVE(CaptureSessionCoordinator);
    ~CaptureSessionCoordinator() override = default;

    /**
     * @brief Returns the currently active session.
     * @return Pointer to the current session or nullptr.
     */
    Pbox::ICaptureSession *getSession() const;

    /**
     * @brief Initializes the coordinator with an idle session.
     */
    void initialize();

    /**
     * @brief Switches the active session.
     * @param new_session The new session to activate.
     */
    void switchToSession(CaptureSessionPtr new_session);

  Q_SIGNALS:
    /**
     * @brief Emitted when the active session finishes.
     */
    void finished();
    /**
     * @brief Emitted when the active session changes.
     */
    void sessionChanged();
    /**
     * @brief Emitted when the active session status changes.
     */
    void statusChanged();
    /**
     * @brief Emitted when the active session capture status changes.
     */
    void captureStatusChanged();

  private Q_SLOTS:
    /**
     * @brief Internal slot for session finished handling.
     */
    void sessionFinished();
    /**
     * @brief Internal slot for session status updates.
     */
    void handleSessionStatusChanged();
    /**
     * @brief Internal slot for capture status updates.
     */
    void handleSessionCaptureStatusChanged();

  private:
    Instance<ICamera> camera_;                                /**< Camera interface used by the session. */
    Instance<CaptureSessionManager> capture_session_manager_; /**< Session factory provider. */
    unique_object_ptr<ICaptureSession> session_{nullptr};     /**< Current active capture session. */
};
} // namespace Pbox

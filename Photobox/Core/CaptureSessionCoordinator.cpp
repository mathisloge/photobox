// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

/**
 * @file CaptureSessionCoordinator.cpp
 * @brief Implementation of capture session lifecycle coordination.
 */

#include "CaptureSessionCoordinator.hpp"
#include "Pbox/Logger.hpp"

DEFINE_LOGGER(capture_session_coordinator);

namespace Pbox
{
CaptureSessionCoordinator::CaptureSessionCoordinator(Instance<ICamera> camera,
                                                     Instance<CaptureSessionManager> capture_session_manager)
    : camera_{std::move(camera)}
    , capture_session_manager_{std::move(capture_session_manager)}
{}

Pbox::ICaptureSession *CaptureSessionCoordinator::getSession() const
{
    return session_.get();
}

void CaptureSessionCoordinator::initialize()
{
    switchToSession(capture_session_manager_->createIdleSession());
}

void CaptureSessionCoordinator::switchToSession(CaptureSessionPtr new_session)
{
    const auto old_session_id = session_ != nullptr ? session_->sessionId() : "unknown";
    session_ = std::move(new_session);

    if (session_ != nullptr)
    {
        connect(session_.get(), &ICaptureSession::requestedImageCapture, camera_.get(), &ICamera::requestCapturePhoto);
        connect(session_.get(), &ICaptureSession::finished, this, &CaptureSessionCoordinator::sessionFinished);
        connect(session_.get(),
                &ICaptureSession::statusChanged,
                this,
                &CaptureSessionCoordinator::handleSessionStatusChanged);
        connect(session_.get(),
                &ICaptureSession::captureStatusChanged,
                this,
                &CaptureSessionCoordinator::handleSessionCaptureStatusChanged);

        handleSessionStatusChanged();
        handleSessionCaptureStatusChanged();
    }

    Q_EMIT sessionChanged();
    LOG_INFO(logger_capture_session_coordinator(),
             "Switched session from '{}' to '{}',",
             old_session_id,
             session_ != nullptr ? session_->sessionId() : "none");
}

void CaptureSessionCoordinator::sessionFinished()
{
    const auto finished_session_id = session_ != nullptr ? session_->sessionId() : "unknown";
    LOG_DEBUG(logger_capture_session_coordinator(), "Session '{}' finished", finished_session_id);
    Q_EMIT finished();

    if (session_ != nullptr && session_->sessionId() != IdleCaptureSession::kName)
    {
        switchToSession(capture_session_manager_->createIdleSession());
    }
}

void CaptureSessionCoordinator::handleSessionStatusChanged()
{
    Q_EMIT statusChanged();
}

void CaptureSessionCoordinator::handleSessionCaptureStatusChanged()
{
    Q_EMIT captureStatusChanged();
}
} // namespace Pbox

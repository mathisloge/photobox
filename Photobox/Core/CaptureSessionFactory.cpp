// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CaptureSessionFactory.hpp"
#include "ICaptureSession.hpp"
#include "IdleCaptureSession.hpp"
#include "Pbox/Logger.hpp"

DEFINE_LOGGER(capture_session_abstract_factory);

namespace Pbox
{
void CaptureSessionManager::registerCaptureSession(CaptureSessionId session_id,
                                                   std::unique_ptr<ICaptureSessionFactory> session_factory)
{
    auto [it, emplaced] = session_factories_.emplace(std::move(session_id), std::move(session_factory));
    if (emplaced)
    {
        LOG_INFO(logger_capture_session_abstract_factory(), "Registered session factory '{}'", it->first);
    }
    else
    {
        LOG_ERROR(logger_capture_session_abstract_factory(), "A session with name '{}' already exists.", it->first);
    }
}

void CaptureSessionManager::addTriggerRelation(CaptureSessionId session_id, TriggerId trigger_id)
{
    auto [it, emplaced] = trigger_session_relation_.emplace(std::move(trigger_id), std::move(session_id));
    if (emplaced)
    {
        LOG_INFO(logger_capture_session_abstract_factory(),
                 "Added a relation between trigger '{}' and session '{}'",
                 it->first,
                 it->second);
    }
    else
    {
        LOG_ERROR(logger_capture_session_abstract_factory(),
                  "A relation between trigger '{}' and session '{}' already exists.",
                  it->first,
                  it->second);
    }
}

void CaptureSessionManager::setInitialCountdown(std::chrono::seconds initial_countdown)
{
    if (initial_countdown_ != initial_countdown)
    {
        initial_countdown_ = initial_countdown;
        LOG_INFO(
            logger_capture_session_abstract_factory(), "Initial countdown set to '{}s'", initial_countdown.count());
    }
}

CaptureSessionPtr CaptureSessionManager::createIdleSession() const
{
    return make_unique_object_ptr_as<ICaptureSession, IdleCaptureSession>();
}

CaptureSessionPtr CaptureSessionManager::createFromTrigger(const TriggerId &trigger_id) const
{
    const auto tid = trigger_session_relation_.find(trigger_id);
    if (tid == trigger_session_relation_.end())
    {
        return nullptr;
    }
    const auto sid = session_factories_.find(tid->second);
    if (sid == session_factories_.end())
    {
        return nullptr;
    }

    return sid->second->create(initial_countdown_);
}

} // namespace Pbox

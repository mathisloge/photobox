// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <functional>
#include "ObjectUniquePtr.hpp"
#include "Pbox/DisableCopyMove.hpp"
#include "TriggerId.hpp"

namespace Pbox
{
class ICaptureSession;

using CaptureSessionPtr = unique_object_ptr<ICaptureSession>;
using CaptureSessionFactoryFnc = std::function<CaptureSessionPtr()>;

class ICaptureSessionFactory
{
  public:
    PBOX_DISABLE_COPY_MOVE(ICaptureSessionFactory);
    ICaptureSessionFactory() = default;
    virtual ~ICaptureSessionFactory() = default;
    virtual CaptureSessionPtr create() const = 0;
};

using CaptureSessionId = std::string;

class CaptureSessionManager
{
  public:
    void registerCaptureSession(CaptureSessionId session_id, std::unique_ptr<ICaptureSessionFactory> session_factory);
    void addTriggerRelation(CaptureSessionId session_id, TriggerId trigger_id);
    CaptureSessionPtr createIdleSession() const;
    CaptureSessionPtr createFromTrigger(const TriggerId &trigger_id) const;

  private:
    std::unordered_map<CaptureSessionId, std::unique_ptr<ICaptureSessionFactory>> session_factories_;
    std::unordered_map<TriggerId, CaptureSessionId> trigger_session_relation_;
};
} // namespace Pbox

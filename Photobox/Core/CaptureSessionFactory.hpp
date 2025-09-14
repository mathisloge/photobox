// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <functional>
#include "ObjectUniquePtr.hpp"
#include "TriggerId.hpp"

namespace Pbox
{
class ICaptureSession;

using CaptureSessionPtr = unique_object_ptr<ICaptureSession>;
using CaptureSessionFactoryFnc = std::function<CaptureSessionPtr()>;

class ICaptureSessionFactory
{
  public:
    CaptureSessionPtr create() const;
};

using CaptureSessionId = std::string;

class CaptureSessionAbstractFactory
{
  public:
    void registerCaptureSession(CaptureSessionId session_id, std::unique_ptr<ICaptureSessionFactory> session_factory);
    void addTriggerRelation(CaptureSessionId session_id, TriggerId trigger_id);
    CaptureSessionPtr createFromTrigger(const TriggerId &trigger_id);

  private:
    std::unordered_map<CaptureSessionId, std::unique_ptr<ICaptureSessionFactory>> session_factories_;
    std::unordered_map<TriggerId, CaptureSessionId> trigger_session_relation_;
};
} // namespace Pbox

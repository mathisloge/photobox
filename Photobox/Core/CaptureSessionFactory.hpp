// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <functional>
#include <Pbox/ObjectUniquePtr.hpp>
#include "Pbox/DisableCopyMove.hpp"

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
    virtual CaptureSessionPtr create(std::chrono::seconds initial_countdown) const = 0;
};

using CaptureSessionId = std::string;
} // namespace Pbox

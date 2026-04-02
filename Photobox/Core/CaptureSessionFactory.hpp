// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QColor>
#include <functional>
#include <Pbox/ObjectUniquePtr.hpp>
#include "Pbox/DisableCopyMove.hpp"

namespace Pbox
{
class ICaptureSession;

using CaptureSessionPtr = unique_object_ptr<ICaptureSession>;
using CaptureSessionFactoryFnc = std::function<CaptureSessionPtr()>;
using CaptureSessionId = std::string;

class ICaptureSessionFactory
{
  public:
    PBOX_DISABLE_COPY_MOVE(ICaptureSessionFactory);
    ICaptureSessionFactory() = default;
    virtual ~ICaptureSessionFactory() = default;
    virtual CaptureSessionPtr create(std::chrono::seconds initial_countdown) const = 0;

    virtual const CaptureSessionId &sessionId() const = 0;
    virtual const std::string &name() const = 0;
    virtual QColor color() const = 0;
};

} // namespace Pbox

// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "ICaptureSession.hpp"

namespace Pbox
{
class CaptureSessionFactory
{};

class CaptureSessionFactoryBuilder;

class CaptureSessionFactoryBuilder
{
    using FactoryFunction = std::function<std::unique_ptr<ICaptureSession>()>;

  public:
    CaptureSessionFactoryBuilder &addIdleState();
    CaptureSessionFactoryBuilder &addState();

    [[nodiscard]] CaptureSessionFactory create();

  private:
    std::optional<FactoryFunction> idle_state_factory_;
};
} // namespace Pbox

// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "CaptureSessionFactory.hpp"
#include "SingleCaptureSession.hpp"

namespace Pbox
{
class SingleCaptureSessionFactory : public ICaptureSessionFactory
{
  public:
    explicit SingleCaptureSessionFactory(std::string name)
        : name_{std::move(name)}
    {}

    CaptureSessionPtr create(std::chrono::seconds initial_countdown) const override
    {
        return make_unique_object_ptr_as<ICaptureSession, SingleCaptureSession>(name_, initial_countdown);
    }

  private:
    std::string name_;
};
} // namespace Pbox

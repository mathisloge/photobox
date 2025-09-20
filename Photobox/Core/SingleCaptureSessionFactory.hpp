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

    CaptureSessionPtr create() const override
    {
        return make_unique_object_ptr_as<ICaptureSession, SingleCaptureSession>(name_);
    }

  private:
    std::string name_;
};
} // namespace Pbox

// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "CaptureSessionFactory.hpp"

namespace Pbox
{
class SingleCaptureSessionFactory : public ICaptureSessionFactory
{
  public:
    explicit SingleCaptureSessionFactory(std::string name);

    CaptureSessionPtr create(std::chrono::seconds initial_countdown) const override;

  private:
    std::string name_;
};
} // namespace Pbox

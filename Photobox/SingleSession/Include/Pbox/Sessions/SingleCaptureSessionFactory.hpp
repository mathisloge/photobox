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
    explicit SingleCaptureSessionFactory(CaptureSessionId session_id, std::string name, QColor color);

    CaptureSessionPtr create(std::chrono::seconds initial_countdown) const override;
    const CaptureSessionId &sessionId() const override;
    const std::string &name() const override;
    QColor color() const override;

  private:
    CaptureSessionId session_id_;
    std::string name_;
    QColor color_;
};
} // namespace Pbox

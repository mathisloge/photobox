// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Pbox/Sessions/SingleCaptureSessionFactory.hpp"
#include "SingleCaptureSession.hpp"
namespace Pbox
{
SingleCaptureSessionFactory::SingleCaptureSessionFactory(std::string name)
    : name_{std::move(name)}
{}

CaptureSessionPtr SingleCaptureSessionFactory::create(std::chrono::seconds initial_countdown) const
{
    return make_unique_object_ptr_as<ICaptureSession, SingleCaptureSession>(name_, initial_countdown);
}

const CaptureSessionId &SingleCaptureSessionFactory::sessionId() const
{
    return name_;
}

const std::string &SingleCaptureSessionFactory::name() const
{
    return name_;
}

QColor SingleCaptureSessionFactory::color() const
{
    return QColor{};
}
} // namespace Pbox

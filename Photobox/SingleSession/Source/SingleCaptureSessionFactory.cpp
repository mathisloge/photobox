// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Pbox/Sessions/SingleCaptureSessionFactory.hpp"
#include "SingleCaptureSession.hpp"

namespace Pbox
{
SingleCaptureSessionFactory::SingleCaptureSessionFactory(CaptureSessionId session_id, std::string name, QColor color)
    : session_id_{std::move(session_id)}
    , name_{std::move(name)}
    , color_{std::move(color)}
{}

CaptureSessionPtr SingleCaptureSessionFactory::create(std::chrono::seconds initial_countdown) const
{
    return make_unique_object_ptr_as<ICaptureSession, SingleCaptureSession>(session_id_, initial_countdown);
}

const CaptureSessionId &SingleCaptureSessionFactory::sessionId() const
{
    return session_id_;
}

const std::string &SingleCaptureSessionFactory::name() const
{
    return name_;
}

QColor SingleCaptureSessionFactory::color() const
{
    return color_;
}
} // namespace Pbox

// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "IdleCaptureSession.hpp"

namespace Pbox
{
IdleCaptureSession::IdleCaptureSession()
    : ICaptureSession{kName}
{
    setLiveViewVisible(true);
    setStatus(ICaptureSession::Status::Idle);
}

void IdleCaptureSession::triggerCapture()
{
    Q_EMIT finished();
}

void IdleCaptureSession::imageCaptured(const QImage &captured_image, std::uint32_t image_id)
{}

void IdleCaptureSession::imageSaved(const std::filesystem::path &captured_image_path)
{}
} // namespace Pbox

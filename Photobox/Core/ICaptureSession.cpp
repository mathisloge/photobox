// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ICaptureSession.hpp"

namespace Pbox
{
ICaptureSession::ICaptureSession(std::string name)
    : name_{std::move(name)}
{}

ICaptureSession::~ICaptureSession() = default;

const std::string &ICaptureSession::name() const
{
    return name_;
}

ICaptureSession::Status ICaptureSession::getStatus() const
{
    return status_;
}

void ICaptureSession::setStatus(ICaptureSession::Status status)
{
    if (status_ != status)
    {
        status_ = status;
        Q_EMIT statusChanged();
    }
}

ICaptureSession::CaptureStatus ICaptureSession::getCaptureStatus() const
{
    return capture_status_;
}

void ICaptureSession::setCaptureStatus(ICaptureSession::CaptureStatus capture_status)
{
    if (capture_status_ != capture_status)
    {
        capture_status_ = capture_status;
        Q_EMIT captureStatusChanged();
    }
}

bool ICaptureSession::isLiveViewVisible() const
{
    return live_view_visible_;
}

void ICaptureSession::setLiveViewVisible(bool visible)
{
    if (visible != live_view_visible_)
    {
        live_view_visible_ = visible;
        Q_EMIT liveViewVisibleChanged();
    }
}

const QString &ICaptureSession::getPreviewImage() const
{
    return preview_image_;
}

void ICaptureSession::setPreviewImage(QString preview_image)
{
    if (preview_image_ != preview_image)
    {
        preview_image_ = std::move(preview_image);
        Q_EMIT previewImageChanged();
    }
}
} // namespace Pbox

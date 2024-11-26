// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ICaptureSession.hpp"

namespace Pbox
{
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

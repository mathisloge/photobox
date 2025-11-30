// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ICamera.hpp"

namespace Pbox
{
QVideoSink *ICamera::getVideoSink() const
{
    return video_sink_;
}

void ICamera::setVideoSink(QVideoSink *video_sink)
{
    if (video_sink_ != video_sink)
    {
        video_sink_ = video_sink;
        if (video_sink_ != nullptr)
        {
            connect(video_sink_, &QVideoSink::destroyed, this, [this]() { setVideoSink(nullptr); });
        }
        Q_EMIT videoSinkChanged();
    }
}

} // namespace Pbox

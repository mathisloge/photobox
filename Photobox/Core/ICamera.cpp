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
        Q_EMIT videoSinkChanged();
    }
}

} // namespace Pbox
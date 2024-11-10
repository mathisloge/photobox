#include "GPhoto2Camera.hpp"
#include <QPointer>
#include <QVideoFrame>
#include <GPhoto2Integration.hpp>
#include <Pbox/Logger.hpp>
#include <Pbox/QStdexec.hpp>
#include <exec/static_thread_pool.hpp>
#include <exec/variant_sender.hpp>

DEFINE_LOGGER(gphoto2camera);

namespace
{
inline auto tst(bool cond, stdexec::sender auto left, stdexec::sender auto right)
    -> exec::variant_sender<decltype(left), decltype(right)>
{
    if (cond)
    {
        return left;
    }
    return right;
};
} // namespace

namespace Pbox
{
GPhoto2Camera::GPhoto2Camera(Scheduler &scheduler)
{

    auto begin = stdexec::schedule(scheduler.getScheduler());
    auto final_flow = stdexec::when_all(begin | Pbox::GPhoto2::flowAutoconnect()) |
                      stdexec::let_value([&scheduler, begin, this](Pbox::GPhoto2::Context &context) {
                          auto capture_flow = stdexec::let_value([this, &context, begin]() {
                              auto capture = begin //
                                             | stdexec::then([this, &context]() {
                                                   capture_photo_ = false;
                                                   return GPhoto2::captureImage(context);
                                               })                                                        //
                                             | stdexec::continues_on(qThreadAsScheduler(this->thread())) //
                                             | stdexec::then([this](auto &&image) {
                                                   if (image.has_value())
                                                   {
                                                       Q_EMIT imageCaptured(image.value());
                                                   }
                                               });
                              auto preview = begin                                        //
                                             | Pbox::GPhoto2::flowCapturePreview(context) //
                                             | stdexec::then([this](auto &&image) { processPreviewImage(image); });

                              return tst(capture_photo_, std::move(capture), std::move(preview));
                          });

                          return begin                                                                    //
                                 | std::move(capture_flow)                                                //
                                 | stdexec::then([this](auto &&...) { return stoken_.stop_requested(); }) //
                                 | exec::repeat_effect_until()                                            //
                                 | stdexec::upon_error([](auto &&error) { LOG_ERROR(gphoto2camera, "GPHOTO2 error"); });
                      }) |
                      stdexec::then([this]() {
                          const bool restart = stoken_.stop_requested();
                          LOG_DEBUG(gphoto2camera, "Camera exited. Restart = {}", restart);
                          return restart;
                      }) |
                      exec::repeat_effect_until();

    async_scope_.spawn(std::move(final_flow));
}

GPhoto2Camera::~GPhoto2Camera()
{
    stoken_.request_stop();
    auto cleanup_sender = async_scope_.on_empty();
    stdexec::sync_wait(cleanup_sender);
}

void GPhoto2Camera::requestCapturePhoto()
{
    capture_photo_ = true;
}

void GPhoto2Camera::processPreviewImage(const QImage &image)
{
    QPointer<QVideoSink> video_sink{getVideoSink()};

    if (video_sink.isNull())
    {
        return;
    }
    QVideoFrame video_frame{QVideoFrameFormat{image.size(), QVideoFrameFormat::Format_RGBA8888}};
    if (video_frame.map(QVideoFrame::WriteOnly))
    {
        uchar *data = video_frame.bits(0); // access Plane 0 for RGB32
        if (data != nullptr)
        {
            const auto converted_image = image.convertedTo(QImage::Format::Format_RGBA8888);
            std::copy(converted_image.bits(), converted_image.bits() + converted_image.sizeInBytes(), data);
        }
        else
        {
            LOG_ERROR(gphoto2camera, "Error while accessing the frame data");
        }

        video_frame.unmap();
    }
    else
    {
        LOG_ERROR(gphoto2camera, "Could not map video frame");
    }

    video_sink->setVideoFrame(video_frame);
}
} // namespace Pbox

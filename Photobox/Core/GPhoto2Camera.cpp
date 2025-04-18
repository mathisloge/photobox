// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "GPhoto2Camera.hpp"
#include <QPointer>
#include <QVideoFrame>
#include <GPhoto2Integration.hpp>
#include <Pbox/CleanupAsyncScope.hpp>
#include <Pbox/Conditional.hpp>
#include <Pbox/Logger.hpp>
#include <Pbox/QStdexec.hpp>
#include <exec/repeat_effect_until.hpp>
#include <exec/static_thread_pool.hpp>
#include "GPhoto2Exeption.hpp"

DEFINE_LOGGER(gphoto2camera);

namespace
{
inline auto flowCapturePreview(Pbox::GPhoto2::Context &context)
{
    return stdexec::then([&context]() -> QImage {
        std::optional<QImage> image;
        int error_count{0};
        while (not image.has_value() and error_count < 5)
        {
            image = Pbox::GPhoto2::capturePreviewImage(context);
            error_count++;
        }
        if (not image.has_value())
        {
            throw Pbox::GPhoto2::GPhoto2Exception{"Could not take preview image"};
        }

        return *image;
    });
}
} // namespace
namespace Pbox
{
GPhoto2Camera::GPhoto2Camera(Scheduler &scheduler)
    : status_client_{QStringLiteral("GPhoto2 Kamera"), true}
{
    auto autoconnect_flow = stdexec::schedule(scheduler.getWorkScheduler()) //
                            | stdexec::then([this]() {
                                  GPhoto2::Context context;
                                  status_client_.setSystemStatus(SystemStatusCode::Code::Connecting);
                                  while (not autodetectAndConnectCamera(context) and not stoken_.stop_requested())
                                  {
                                  }
                                  return context;
                              }) |
                            stdexec::continues_on(scheduler.getQtEventLoopScheduler()) |
                            stdexec::then([this](auto &&context) {
                                status_client_.setSystemStatus(SystemStatusCode::Code::Ok);
                                return context;
                            });
    //! TODO: stop source does not really work :D
    auto final_flow = stdexec::when_all(std::move(autoconnect_flow)) //
                      | stdexec::let_value([&scheduler, this](Pbox::GPhoto2::Context &context) {
                            auto capture_flow = stdexec::let_value([this, &context, &scheduler]() {
                                auto capture =
                                    stdexec::schedule(scheduler.getWorkScheduler()) //
                                    | stdexec::then([this, &context]() {
                                          capture_photo_ = false;
                                          return GPhoto2::captureImage(context);
                                      })                                                         //
                                    | stdexec::continues_on(scheduler.getQtEventLoopScheduler()) //
                                    | stdexec::then([this](auto &&image) {
                                          if (image.has_value())
                                          {
                                              Q_EMIT imageCaptured(image.value());
                                          }
                                          return image.has_value();
                                      })                           //
                                    | exec::repeat_effect_until(); // todo: this now retries forever to capture a image.
                                                                   // maybe retry_n times and emit an error?
                                auto preview = stdexec::schedule(scheduler.getWorkScheduler()) //
                                               | flowCapturePreview(context)                   //
                                               | stdexec::then([this](auto &&image) { processPreviewImage(image); });

                                return conditional(capture_photo_, std::move(capture), std::move(preview));
                            });

                            return stdexec::schedule(scheduler.getWorkScheduler())                          //
                                   | std::move(capture_flow)                                                //
                                   | stdexec::then([this](auto &&...) { return stoken_.stop_requested(); }) //
                                   | exec::repeat_effect_until()                                            //
                                   | stdexec::upon_error([this](auto &&error) {
                                         status_client_.setSystemStatus(SystemStatusCode::Code::Error);
                                         LOG_ERROR(gphoto2camera, "GPHOTO2 error");
                                     });
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
    LOG_DEBUG(gphoto2camera, "Camera stop");
    stoken_.request_stop();
    cleanup_async_scope(async_scope_);

    LOG_DEBUG(gphoto2camera, "Async scope cleaned up");
}

void GPhoto2Camera::requestCapturePhoto()
{
    capture_photo_ = true;
}

const SystemStatusClient &GPhoto2Camera::systemStatusClient() const
{
    return status_client_;
}

void GPhoto2Camera::processPreviewImage(const QImage &image)
{
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

    QPointer<QVideoSink> video_sink{getVideoSink()};
    if (video_sink.isNull())
    {
        return;
    }
    video_sink->setVideoFrame(video_frame);
}
} // namespace Pbox

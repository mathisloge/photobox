// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
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
#include "GPhoto2Exeption.hpp"

#undef emit // stupid qt...
#include <asio/steady_timer.hpp>
#include <asioexec/use_sender.hpp>
#include <exec/env.hpp>
#include <exec/repeat_effect_until.hpp>
#include <exec/static_thread_pool.hpp>

DEFINE_LOGGER(gphoto2camera);
namespace Pbox
{
namespace
{
auto flowCapturePreview(Pbox::GPhoto2::Context &context)
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

GPhoto2Camera::GPhoto2Camera(Scheduler &scheduler)
    : scheduler_{scheduler}
    , status_client_{QStringLiteral("GPhoto2 Kamera"), true}
{
    async_scope_.spawn(asyncCaptureLoop());
}

exec::task<void> GPhoto2Camera::asyncCaptureLoop()
{
    auto token = co_await stdexec::get_stop_token();
    while (not token.stop_requested())
    {
        auto context = co_await stdexec::stopped_as_optional(asyncAutoconnect());
        if (not context.has_value())
        {
            continue;
        }
        LOG_DEBUG(gphoto2camera, "Got a camera...");

        auto capture = stdexec::schedule(scheduler_.getWorkScheduler()) //
                       | stdexec::then([this, &context]() {
                             LOG_DEBUG(gphoto2camera, "capture image...");
                             capture_photo_ = false;
                             return GPhoto2::captureImage(*context);
                         })                                                          //
                       | stdexec::continues_on(scheduler_.getQtEventLoopScheduler()) //
                       | stdexec::then([this](auto &&image) {
                             if (image.has_value())
                             {
                                 Q_EMIT imageCaptured(image.value());
                             }
                             return image.has_value();
                         })                           //
                       | exec::repeat_effect_until(); // todo: this now retries forever to capture a image.
                                                      // maybe retry_n times and emit an error?
        auto preview = stdexec::schedule(scheduler_.getWorkScheduler()) //
                       | flowCapturePreview(*context)                   //
                       | stdexec::then([this](auto &&image) { processPreviewImage(image); });

        while (status_client_.systemStatus() == SystemStatusCode::Code::Ok and not token.stop_requested())
        {
            co_await stdexec::starts_on(exec::inline_scheduler{},
                                        conditional(capture_photo_, capture, preview) |
                                            stdexec::upon_error([this](auto &&error) {
                                                status_client_.setSystemStatus(SystemStatusCode::Code::Error);
                                                try
                                                {
                                                    if (error)
                                                    {
                                                        std::rethrow_exception(error);
                                                    }
                                                }
                                                catch (const std::exception &e)
                                                {
                                                    LOG_ERROR(gphoto2camera, "GPhoto2 Exception: {}", e.what());
                                                }
                                            }));
        }
    }
}

exec::task<GPhoto2::Context> GPhoto2Camera::asyncAutoconnect()
{
    GPhoto2::Context context;
    status_client_.setSystemStatus(SystemStatusCode::Code::Connecting);

    while (not autodetectAndConnectCamera(context))
    {
        co_await asio::steady_timer{scheduler_.getWorkExecutor(), std::chrono::milliseconds{100}}.async_wait(
            asioexec::use_sender);
    }
    status_client_.setSystemStatus(SystemStatusCode::Code::Ok);
    co_return context;
}

GPhoto2Camera::~GPhoto2Camera()
{
    LOG_DEBUG(gphoto2camera, "Camera stop");
    async_scope_.request_stop();
    cleanup_async_scope(async_scope_);

    LOG_DEBUG(gphoto2camera, "Async scope cleaned up");
}

void GPhoto2Camera::requestCapturePhoto()
{
    LOG_DEBUG(gphoto2camera, "image requested...");
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

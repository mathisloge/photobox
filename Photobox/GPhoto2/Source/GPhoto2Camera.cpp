// SPDX-FileCopyrightText: 2024 - 2026 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Pbox/GPhoto2Camera.hpp"
#include <QPointer>
#include <QVideoFrame>
#include <Pbox/CleanupAsyncScope.hpp>
#include <Pbox/Logger.hpp>
#include <Pbox/QStdexec.hpp>
#include "GPhoto2Context.hpp"
#include "GPhoto2Integration.hpp"
#include "Pbox/GPhoto2Exception.hpp"

#undef emit // stupid qt...
#include <asioexec/use_sender.hpp>
#include <boost/asio/steady_timer.hpp>
#include <exec/env.hpp>
#include <exec/static_thread_pool.hpp>

DEFINE_LOGGER(gphoto2_camera);

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
            throw GPhoto2Exception{"Could not take preview image"};
        }

        return *image;
    });
}
} // namespace

GPhoto2Camera::GPhoto2Camera(Instance<Scheduler> scheduler)
    : scheduler_{std::move(scheduler)}
    , status_client_{QStringLiteral("GPhoto2 Kamera"), true}
{
    async_scope_.spawn(asyncCaptureLoop());
}

exec::task<void> GPhoto2Camera::asyncCaptureLoop()
{
    auto token = co_await stdexec::get_stop_token();
    while (not token.stop_requested())
    {
        auto context_opt = co_await stdexec::stopped_as_optional(asyncAutoconnect());
        if (not context_opt.has_value())
        {
            continue;
        }
        LOG_DEBUG(logger_gphoto2_camera(), "Got a camera...");

        GPhoto2::Context &context = *context_opt;

        while (status_client_.systemStatus() == SystemStatusCode::Code::Ok and not token.stop_requested())
        {
            try
            {
                if (capture_photo_.exchange(false, std::memory_order_relaxed))
                {
                    LOG_DEBUG(logger_gphoto2_camera(), "Triggering capture...");
                    co_await (stdexec::schedule(scheduler_->getWorkScheduler()) |
                              stdexec::then([&context]() { GPhoto2::triggerCapture(context); }));
                }

                const auto events =
                    co_await (stdexec::schedule(scheduler_->getWorkScheduler()) |
                              stdexec::then([&context]() { return GPhoto2::pollCameraEvents(context); }));

                for (const auto &event : events)
                {
                    if (event.type == GP_EVENT_FILE_ADDED)
                    {
                        LOG_DEBUG(logger_gphoto2_camera(),
                                  "File added: {}/{}",
                                  event.camera_file_path.folder,
                                  event.camera_file_path.name);

                        auto image = co_await (stdexec::schedule(scheduler_->getWorkScheduler()) |
                                               stdexec::then([&context, &event]() {
                                                   return GPhoto2::downloadImage(context, event.camera_file_path);
                                               }));

                        if (image)
                        {
                            co_await (stdexec::schedule(scheduler_->getQtEventLoopScheduler()) |
                                      stdexec::then(
                                          [this, image = std::move(image)]() { Q_EMIT imageCaptured(image.value()); }));
                        }
                    }
                    else if (event.type == GP_EVENT_CAPTURE_COMPLETE)
                    {
                        LOG_DEBUG(logger_gphoto2_camera(), "Camera finished the capture. Waiting for a file added event...");
                    }
                }
                auto preview_image =
                    co_await (stdexec::schedule(scheduler_->getWorkScheduler()) | flowCapturePreview(context));

                co_await (stdexec::schedule(scheduler_->getQtEventLoopScheduler()) |
                          stdexec::then([this, preview_image = std::move(preview_image)]() {
                              processPreviewImage(preview_image);
                          }));
            }
            catch (const std::exception &e)
            {
                LOG_ERROR(logger_gphoto2_camera(), "GPhoto2 Exception: {}", e.what());
                status_client_.setSystemStatus(SystemStatusCode::Code::Error);
                break;
            }
        }
    }
}

exec::task<GPhoto2::Context> GPhoto2Camera::asyncAutoconnect()
{
    GPhoto2::Context context;
    status_client_.setSystemStatus(SystemStatusCode::Code::Connecting);

    while (not autodetectAndConnectCamera(context))
    {
        co_await boost::asio::steady_timer{scheduler_->getWorkExecutor(), std::chrono::milliseconds{100}}.async_wait(
            asioexec::use_sender);
    }
    status_client_.setSystemStatus(SystemStatusCode::Code::Ok);
    co_return context;
}

GPhoto2Camera::~GPhoto2Camera()
{
    LOG_DEBUG(logger_gphoto2_camera(), "Camera stop");
    async_scope_.request_stop();
    cleanup_async_scope(async_scope_);

    LOG_DEBUG(logger_gphoto2_camera(), "Async scope cleaned up");
}

void GPhoto2Camera::requestCapturePhoto()
{
    LOG_DEBUG(logger_gphoto2_camera(), "image requested...");
    capture_photo_.store(true, std::memory_order_relaxed);
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
            LOG_ERROR(logger_gphoto2_camera(), "Error while accessing the frame data");
        }

        video_frame.unmap();
    }
    else
    {
        LOG_ERROR(logger_gphoto2_camera(), "Could not map video frame");
    }

    QPointer<QVideoSink> video_sink{getVideoSink()};
    if (video_sink.isNull())
    {
        return;
    }
    video_sink->setVideoFrame(video_frame);
}
} // namespace Pbox

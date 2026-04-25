// SPDX-FileCopyrightText: 2026 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "CapturePipeline.hpp"
#include <Pbox/CleanupAsyncScope.hpp>
#include <Pbox/Logger.hpp>

DEFINE_LOGGER(capture_pipeline);

namespace Pbox
{
CapturePipeline::CapturePipeline(Instance<Scheduler> scheduler,
                                 Instance<ImageStorage> image_storage,
                                 std::function<ICaptureSession *()> current_session_callback)
    : scheduler_{std::move(scheduler)}
    , image_storage_{std::move(image_storage)}
    , current_session_callback_{std::move(current_session_callback)}
{}

CapturePipeline::~CapturePipeline()
{
    cleanup_async_scope(async_scope_);
}

void CapturePipeline::onCameraImageCaptured(const QImage &image)
{
    const auto image_id = image_ids_++;
    Q_EMIT imageCaptured(image, image_id);

    if (auto *session = current_session_callback_())
    {
        session->imageCaptured(image, image_id);
    }

    async_scope_.spawn(stdexec::schedule(scheduler_->getWorkScheduler()) |
                       stdexec::then([image, this]() { return image_storage_->saveImage(image); }) |
                       stdexec::continues_on(scheduler_->getQtEventLoopScheduler()) |
                       stdexec::then([this](auto &&saved_image_path) {
                           if (auto *session = current_session_callback_())
                           {
                               session->imageSaved(saved_image_path);
                           }
                       }) |
                       stdexec::upon_error([](auto &&ex_ptr) {
                           try
                           {
                               std::rethrow_exception(ex_ptr);
                           }
                           catch (const std::exception &ex)
                           {
                               LOG_ERROR(logger_capture_pipeline(), "Error while saving image. {}", ex.what());
                           }
                       }));
}
} // namespace Pbox

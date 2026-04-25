// SPDX-FileCopyrightText: 2026 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QImage>
#include <QObject>
#include <atomic>
#include <functional>
#include <Pbox/Instance.hpp>
#include <exec/async_scope.hpp>
#include "ICaptureSession.hpp"
#include "ImageStorage.hpp"
#include "Scheduler.hpp"

namespace Pbox
{
/**
 * @brief Handles the image capture and persistence pipeline.
 *
 * CapturePipeline receives camera images, emits preview signals,
 * saves images on a background scheduler, and notifies the active session
 * when image saving completes.
 */
class CapturePipeline : public QObject
{
    Q_OBJECT

  public:
    /**
     * @brief Constructs the capture pipeline.
     * @param scheduler Scheduler used for async work.
     * @param image_storage Image storage used to persist captures.
     * @param current_session_callback Callback returning the active session.
     * @param parent QObject parent.
     */
    explicit CapturePipeline(Instance<Scheduler> scheduler,
                             Instance<ImageStorage> image_storage,
                             std::function<ICaptureSession *()> current_session_callback);
    PBOX_DISABLE_COPY_MOVE(CapturePipeline);
    /**
     * @brief Destroys the capture pipeline and cancels pending saves.
     */
    ~CapturePipeline() override;

  Q_SIGNALS:
    /**
     * @brief Emitted when a new captured image is ready for preview.
     * @param image Captured image.
     * @param image_id Sequential image identifier.
     */
    void imageCaptured(const QImage &image, std::uint32_t image_id);

  public Q_SLOTS:
    /**
     * @brief Handles a new image from the camera.
     * @param image Captured image.
     */
    void onCameraImageCaptured(const QImage &image);

  private:
    Instance<Scheduler> scheduler_;                               /**< Scheduler for background save tasks. */
    Instance<ImageStorage> image_storage_;                        /**< Storage backend for captured images. */
    std::function<ICaptureSession *()> current_session_callback_; /**< Callback to get the active session. */
    exec::async_scope async_scope_;                               /**< Async scope for pending save operations. */
    std::atomic_uint32_t image_ids_{0};                           /**< Sequential preview image IDs. */
};
} // namespace Pbox

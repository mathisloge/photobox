// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <Pbox/Instance.hpp>
#include <exec/async_scope.hpp>
#include <exec/task.hpp>
#include "ICamera.hpp"
#include "Scheduler.hpp"
#include "SystemStatusClient.hpp"

namespace Pbox
{
namespace GPhoto2
{
struct Context;
}

/**
 * @brief Camera implementation backed by a gphoto2 device.
 *
 * This class owns the asynchronous capture loop, manages camera
 * auto-connection, polls for camera events, downloads captured images,
 * and forwards preview frames to the UI layer.
 *
 * The object is non-copyable and non-movable because it manages an active
 * async scope and Qt-bound resources.
 */
class GPhoto2Camera : public ICamera
{
    Q_OBJECT

  public:
    Q_DISABLE_COPY_MOVE(GPhoto2Camera);

    /**
     * @brief Constructs a gphoto2 camera controller.
     *
     * The capture loop is started immediately and continues running until
     * destruction or stop request.
     *
     * @param scheduler Scheduler instance used to dispatch work-thread and Qt-loop tasks.
     */
    GPhoto2Camera(Instance<Scheduler> scheduler);

    /**
     * @brief Stops all asynchronous activity and releases camera resources.
     */
    ~GPhoto2Camera() override;

    /**
     * @brief Requests that the next capture cycle triggers a photo capture.
     *
     * The request is handled asynchronously by the internal capture loop.
     */
    void requestCapturePhoto() override;

    /**
     * @brief Returns the status client used to report camera connection state.
     *
     * @return Read-only reference to the system status client.
     */
    const SystemStatusClient &systemStatusClient() const override;

  private:
    /**
     * @brief Converts a preview image into a video frame and forwards it to the UI sink.
     *
     * @param image Preview image retrieved from the camera.
     */
    void processPreviewImage(const QImage &image);

    /**
     * @brief Main asynchronous camera processing loop.
     *
     * The loop repeatedly attempts to connect to a camera, polls for events,
     * downloads newly added files, and updates preview frames while the
     * system status remains healthy.
     *
     * @return A task representing the long-running capture loop.
     */
    exec::task<void> asyncCaptureLoop();

    /**
     * @brief Repeatedly attempts to autodetect and connect a camera.
     *
     * The function updates the system status while connecting and retries
     * until a camera becomes available.
     *
     * @return A task that produces a fully initialized gphoto2 context.
     */
    exec::task<GPhoto2::Context> asyncAutoconnect();

  private:
    /** @brief Scheduler used to run work-thread and Qt-event-loop continuations. */
    Instance<Scheduler> scheduler_;

    /** @brief Publishes connection and runtime status to the rest of the system. */
    SystemStatusClient status_client_;

    /** @brief Owns all asynchronous work spawned by this camera instance. */
    exec::async_scope async_scope_;

    /** @brief Set when a capture request has been issued by the caller. */
    std::atomic_bool capture_photo_{false};
};

} // namespace Pbox

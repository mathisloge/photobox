// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <optional>
#include <vector>
#include <gphoto2/gphoto2-camera.h>

class QImage;

namespace Pbox::GPhoto2
{

/**
 * @brief Represents an event emitted by a connected camera.
 *
 * This structure provides a simplified representation of gphoto2 camera
 * events so they can be processed without directly depending on raw
 * gphoto2 event handling.
 */
struct CameraEvent {

    /**
     * @brief Type of the camera event.
     */
    CameraEventType type{};

    /**
     * @brief Path to the affected file on the camera.
     *
     * This field is populated for events that reference files or folders
     * (for example GP_EVENT_FILE_ADDED or GP_EVENT_FOLDER_ADDED).
     * For unrelated events this remains default initialized.
     */
    CameraFilePath camera_file_path{};
};


struct Context;

/**
 * @brief Delete downloaded images from the camera after transfer.
 */
inline constexpr bool kDeletePhotoFromCamera = true;

/**
 * @brief Keep downloaded images stored on the camera.
 */
inline constexpr bool kKeepPhotoOnCamera = false;

/**
 * @brief Automatically detects and connects to the first available camera.
 *
 * This function performs:
 * - Camera autodetection via gphoto2
 * - Camera ability resolution
 * - Port configuration
 * - Camera object initialization
 *
 * On success, the camera is stored inside the provided context.
 *
 * @param context Initialized gphoto2 context
 *
 * @retval true  Camera successfully detected and configured
 * @retval false No camera found or initialization failed
 */
bool autodetectAndConnectCamera(Context &context);

/**
 * @brief Triggers the camera to capture an image.
 *
 * This function only sends the capture command and returns immediately.
 * Image retrieval must be performed separately (pollCameraEvents() and downloadImage())
 *
 * @param context Connected camera context
 *
 * @throws GPhoto2Exception if the capture command fails
 */
void triggerCapture(Context &context);

/**
 * @brief Downloads an image from the camera storage.
 *
 * The image is decoded into a QImage. Optionally, the file can be deleted
 * from the camera after a successful transfer.
 *
 * @param context Connected camera context
 * @param camera_file_path Path of the image on the camera
 * @param delete_from_camera If true, removes the image after download
 *
 * @return Downloaded image on success, std::nullopt on failure
 *
 * @throws GPhoto2Exception if deletion was requested but failed
 */
std::optional<QImage> downloadImage(
    Context &context,
    CameraFilePath camera_file_path,
    bool delete_from_camera = kDeletePhotoFromCamera);

/**
 * @brief Polls and retrieves all pending camera events.
 *
 * This function drains the camera event queue until a timeout event
 * indicates that no more events are currently pending.
 *
 * Typical usage includes detecting when new images become available
 * after triggering capture.
 *
 * @param context Connected camera context
 *
 * @return Vector containing all collected camera events
 *
 * @throws GPhoto2Exception if event polling fails
 */
std::vector<CameraEvent> pollCameraEvents(Context& context);

/**
 * @brief Captures a low-resolution preview image from the camera.
 *
 * @param context Connected camera context
 *
 * @return Preview image on success, std::nullopt otherwise
 */
std::optional<QImage> capturePreviewImage(Context &context);

} // namespace Pbox::GPhoto2

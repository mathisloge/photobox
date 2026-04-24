// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "ICaptureSession.hpp"

namespace Pbox
{
/**
 * @brief Idle capture session implementation.
 *
 * This class implements an idle session that does nothing when triggered.
 * It serves as a placeholder when no active capture mode is selected.
 */
class IdleCaptureSession : public ICaptureSession
{
    Q_OBJECT
  public:
    static constexpr std::string kName = "IdleSession"; /**< The name of the idle session. */

    IdleCaptureSession();
    /**
     * @brief Triggers the session (does nothing).
     */
    void triggerCapture() override;
    /**
     * @brief Handles image capture (discards the image).
     * @param captured_image The captured image.
     * @param image_id The image ID.
     */
    void imageCaptured(const QImage &captured_image, std::uint32_t image_id) override;
    /**
     * @brief Handles image saving (does nothing).
     * @param captured_image_path The path to the saved image.
     */
    void imageSaved(const std::filesystem::path &captured_image_path) override;
};
} // namespace Pbox

// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <filesystem>
#include <string>
#include <vector>
namespace Pbox
{
/**
 * @brief Configuration structure for collage capture sessions.
 *
 * This struct holds settings specific to collage sessions, including
 * the SVG template, automatic capture options, and image element IDs.
 */
struct CollageSettings
{
    std::filesystem::path svg_template;        /**< Path to the SVG template file. */
    bool automatic_capture{};                  /**< Whether to capture automatically. */
    std::chrono::seconds time_between_capture; /**< Time between automatic captures. */
    std::vector<std::string> image_elements;   /**< IDs of image elements in the template. */
};
} // namespace Pbox

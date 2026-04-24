// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <filesystem>
#include <optional>
#include <string>
#include <vector>
#include <CollageSettings.hpp>
#include "RemoteTriggerId.hpp"

namespace Pbox
{
/**
 * @brief Configuration for ESPHome remote trigger.
 */
struct EspHomeRemoteTriggerConfig
{
    RemoteTriggerId name;
    std::string uri;
};

/**
 * @brief Configuration for camera LED.
 */
struct CameraLedConfig
{
    std::string uri;
};

/**
 * @brief Enumeration of session types.
 */
enum class SessionType
{
    Unknown,       // Unknown session type.
    SingleCapture, // Single capture session.
    CollageCapture // Collage capture session.
};

/**
 * @brief Configuration for a session.
 */
struct SessionConfig
{
    std::string id;
    std::string name;
    SessionType type{};
    bool print{};
    std::vector<std::string> triggers;
    std::optional<CollageSettings> collage;
    std::string color_hex;
};

/**
 * @brief Configuration for fonts.
 */
struct FontConfig
{
    std::string family;
    std::filesystem::path path;
    bool bold;
    bool italic;
};

struct ProjectConfig
{
    std::string name;
    std::filesystem::path capture_dir;
    std::chrono::seconds initial_countdown;
    std::optional<CameraLedConfig> camera_led;
    std::vector<EspHomeRemoteTriggerConfig> remote_triggers;
    std::vector<SessionConfig> sessions;
    std::vector<FontConfig> fonts;
};

} // namespace Pbox

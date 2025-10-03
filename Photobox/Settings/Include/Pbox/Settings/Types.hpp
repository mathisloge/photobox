// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include <CollageSettings.hpp>

namespace Pbox
{
using RemoteTriggerId = std::string;
struct EspHomeRemoteTriggerConfig
{
    RemoteTriggerId name;
    std::string uri;
};

struct CameraLedConfig
{
    std::string uri;
};

enum class SessionType
{
    Unknown,
    SingleCapture,
    CollageCapture
};

struct SessionConfig
{
    std::string name;
    SessionType type{};
    bool print{};
    std::vector<std::string> triggers;
    std::optional<CollageSettings> collage;
};

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

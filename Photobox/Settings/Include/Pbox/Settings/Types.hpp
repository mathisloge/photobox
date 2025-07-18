// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathis.opensource@tuta.io>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <filesystem>
#include <string>
#include <vector>

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

enum class ConditionType
{
    Unknown,
    DisplayTouch,
    RemoteTrigger
};

struct TriggerCondition
{
    ConditionType type;
    std::string id;
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
    SessionType type;
    std::vector<TriggerCondition> trigger_conditions;
};

struct ProjectConfig
{
    std::string name;
    std::filesystem::path capture_dir;
    std::optional<CameraLedConfig> camera_led;
    std::vector<EspHomeRemoteTriggerConfig> remote_triggers;
    std::vector<SessionConfig> sessions;
};

} // namespace Pbox

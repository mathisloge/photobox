// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <nlohmann/json.hpp>

namespace nlohmann
{
template <typename T>
struct adl_serializer<std::optional<T>>
{
    static void from_json(const json &j, std::optional<T> &opt)
    {
        if (j.is_null())
        {
            opt = std::nullopt;
        }
        else
        {
            opt = j.get<T>();
        }
    }
    static void to_json(json &json, const std::optional<T> &t)
    {
        if (t.has_value())
        {
            json = json = t.value();
        }
        else
        {
            json = nullptr;
        }
    }
};
} // namespace nlohmann

namespace Pbox
{
using RemoteTriggerId = std::string;
struct RemoteTriggerConfig
{
    RemoteTriggerId name;
    std::string uri;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RemoteTriggerConfig, name, uri);

struct CameraLedConfig
{
    std::string uri;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CameraLedConfig, uri);

enum class ConditionType
{
    Unknown,
    DisplayTouch,
    RemoteTrigger
};
NLOHMANN_JSON_SERIALIZE_ENUM(ConditionType,
                             {
                                 {ConditionType::Unknown, nullptr},
                                 {ConditionType::DisplayTouch, "display-touch"},
                                 {ConditionType::RemoteTrigger, "remote-trigger"},
                             });

struct TriggerCondition
{
    ConditionType type;
    std::string id;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TriggerCondition, type, id);

enum class SessionType
{
    Unknown,
    SingleCapture,
    CollageCapture
};
NLOHMANN_JSON_SERIALIZE_ENUM(SessionType,
                             {
                                 {SessionType::Unknown, nullptr},
                                 {SessionType::SingleCapture, "single-capture"},
                                 {SessionType::CollageCapture, "collage-capture"},
                             });

struct SessionConfig
{
    std::string name;
    SessionType type;
    std::vector<TriggerCondition> trigger_conditions;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SessionConfig, name, trigger_conditions);

struct ProjectConfig
{
    std::string name;
    std::filesystem::path capture_dir;
    std::optional<CameraLedConfig> camera_led;
    std::vector<RemoteTriggerConfig> remote_triggers;
    std::vector<SessionConfig> sessions;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ProjectConfig, name, capture_dir, camera_led, remote_triggers, sessions);

} // namespace Pbox

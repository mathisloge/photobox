// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
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
        opt = j.is_null() ? std::nullopt : j.get<T>();
    }
    static void to_json(json &json, std::optional<T> t)
    {
        json = t.value_or(nullptr);
    }
};
} // namespace nlohmann
namespace Pbox
{

struct RemoteTriggerConfig
{
    std::string name;
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

struct SessionConfig
{

    std::string name;
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

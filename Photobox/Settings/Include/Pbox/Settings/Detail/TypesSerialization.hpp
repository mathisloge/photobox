// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <Pbox/Settings/Types.hpp>
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
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EspHomeRemoteTriggerConfig, name, uri);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CameraLedConfig, uri);
NLOHMANN_JSON_SERIALIZE_ENUM(SessionType,
                             {
                                 {SessionType::Unknown, nullptr},
                                 {SessionType::SingleCapture, "SingleCapture"},
                                 {SessionType::CollageCapture, "CollageCapture"},
                             });
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SessionConfig, name, type, print, triggers);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ProjectConfig, name, capture_dir, camera_led, remote_triggers, sessions);

} // namespace Pbox

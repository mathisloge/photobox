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

template <typename Rep, typename Period>
struct adl_serializer<std::chrono::duration<Rep, Period>>
{
    static void from_json(const json &j, std::chrono::duration<Rep, Period> &duration)
    {
        duration = std::chrono::duration<Rep, Period>{j.get<int64_t>()};
    }

    static void to_json(json &j, const std::chrono::duration<Rep, Period> &duration)
    {
        j = duration.count();
    }
};

} // namespace nlohmann

namespace Pbox
{
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FontConfig, family, path, bold, italic);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EspHomeRemoteTriggerConfig, name, uri);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(CameraLedConfig, uri);
NLOHMANN_JSON_SERIALIZE_ENUM(SessionType,
                             {
                                 {SessionType::Unknown, nullptr},
                                 {SessionType::SingleCapture, "SingleCapture"},
                                 {SessionType::CollageCapture, "CollageCapture"},
                             });
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    CollageSettings, automatic_capture, svg_template, image_elements, time_between_capture)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SessionConfig, name, type, print, triggers, collage);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    ProjectConfig, name, capture_dir, initial_countdown, camera_led, remote_triggers, sessions, fonts);

} // namespace Pbox

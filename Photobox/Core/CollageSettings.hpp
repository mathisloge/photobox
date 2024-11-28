// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <nlohmann/json.hpp>
namespace Pbox
{
struct CollageSettings
{
    bool automatic_capture{true};
    int seconds_between_capture{5};
    std::vector<std::string> image_elements;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CollageSettings, automatic_capture, seconds_between_capture, image_elements);
};
} // namespace Pbox

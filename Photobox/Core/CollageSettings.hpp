// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <filesystem>
#include <string>
#include <vector>
namespace Pbox
{
struct CollageSettings
{
    std::filesystem::path svg_template;
    bool automatic_capture{};
    std::chrono::seconds time_between_capture;
    std::vector<std::string> image_elements;
};
} // namespace Pbox

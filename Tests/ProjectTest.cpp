// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Project.hpp>
#include <catch2/catch_test_macros.hpp>
#include "TestAssets.hpp"

using namespace Pbox;

TEST_CASE("Project test", "[settings]")
{
    Project project{std::filesystem::path{kAssetsPath} / "TestProject.json"};

    REQUIRE(project.name() == "hochzeit-xyz");
}

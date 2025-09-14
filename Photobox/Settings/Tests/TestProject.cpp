// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Pbox/Settings/Project.hpp>
#include <catch2/catch_test_macros.hpp>
#include "SystemStatusManager.hpp"
#include "TestAssets.hpp"

using namespace Pbox;

TEST_CASE("Project test", "[settings]")
{
    Project project{std::make_shared<TriggerManager>(std::make_shared<SystemStatusManager>())};
    project.initFromConfig(std::filesystem::path{kAssetsPath} / "ProjectSettings.json");
    REQUIRE(project.name() == "hochzeit-xyz");
}

TEST_CASE("Project create from non existing file", "[settings]")
{
    Project project{std::make_shared<TriggerManager>(std::make_shared<SystemStatusManager>())};

    REQUIRE(project.name() == "");
}

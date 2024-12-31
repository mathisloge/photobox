#include <catch2/catch_test_macros.hpp>
#include <Project.hpp>
#include "TestAssets.hpp"

using namespace Pbox;

TEST_CASE( "Project test", "[settings]" ) {
    Project project{std::filesystem::path{kAssetsPath} / "TestProject.json"};

    

}

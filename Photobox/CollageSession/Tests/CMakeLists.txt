# SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
#
# SPDX-License-Identifier: GPL-3.0-or-later

if(NOT TARGET quite::core)
    message(STATUS "quite not found: skipping CollageSessionTests")
    return()
endif()

add_executable(CollageSessionTests main.cpp TestCollageRenderer.cpp)
target_link_libraries(
    CollageSessionTests
    PRIVATE Qt6::Test Catch2::Catch2 Photobox::CollageSession quite::core
)
target_include_directories(
    CollageSessionTests
    PRIVATE "${CMAKE_CURRENT_BINARY_DIR}"
)
catch_discover_tests(CollageSessionTests)

set(TestAssetsPath "${CMAKE_CURRENT_SOURCE_DIR}/Assets")
configure_file(TestAssets.hpp.in TestAssets.hpp)

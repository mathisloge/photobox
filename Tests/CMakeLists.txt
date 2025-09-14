# SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
#
# SPDX-License-Identifier: GPL-3.0-or-later

add_executable(
    tests
    main.cpp
    # CollageRendererTest.cpp
    # TestEspHomeRemoteTrigger.cpp
    # pixel_match.cpp
    # pixel_match.hpp
)
#add_subdirectory(CaptureSessions)
target_link_libraries(
    tests
    PRIVATE
        Qt6::Test
        Catch2::Catch2
        PhotoboxCore
        PhotoboxPeripheral
        PhotoboxGPhoto2
)
target_include_directories(tests PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
catch_discover_tests(tests)

set(asset_path "${CMAKE_CURRENT_SOURCE_DIR}/assets")
configure_file(TestAssets.hpp.in TestAssets.hpp)

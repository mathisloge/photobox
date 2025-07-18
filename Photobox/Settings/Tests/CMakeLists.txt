add_executable(SettingsTests
    main.cpp
    TestProject.cpp
)
target_link_libraries(SettingsTests PRIVATE
    Qt6::Test
    Catch2::Catch2
    Photobox::Settings
)
target_include_directories(SettingsTests PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
catch_discover_tests(SettingsTests)

set(TestAssetsPath "${CMAKE_CURRENT_SOURCE_DIR}/Assets")
configure_file(TestAssets.hpp.in TestAssets.hpp)

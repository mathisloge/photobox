find_package(Catch2 3 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Test)

add_executable(tests
    main.cpp
    CollageRendererTest.cpp
    TestEspHomeRemoteTrigger.cpp
    pixel_match.cpp
    pixel_match.hpp
    GPhoto2IntegrationTest.cpp
)
target_link_libraries(tests PRIVATE
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

find_package(Catch2 3 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Test)

add_executable(tests
    main.cpp
    CollageRendererTest.cpp
    TestEspHomeRemoteTrigger.cpp
)
target_link_libraries(tests PRIVATE
    Qt6::Test
    Catch2::Catch2
    PhotoboxCore
    PhotoboxPeripheral
)
catch_discover_tests(tests)

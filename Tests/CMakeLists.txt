find_package(Catch2 3 REQUIRED)


add_executable(tests
    CollageRendererTest.cpp

)
target_link_libraries(tests PRIVATE
    Catch2::Catch2WithMain
    PhotoboxCore
)
catch_discover_tests(tests)

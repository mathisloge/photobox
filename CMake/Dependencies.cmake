macro(declare_photobox_dependencies)
    set(LUNASVG_BUILD_EXAMPLES OFF)
    FetchContent_Declare(
        lunasvg
        GIT_REPOSITORY https://github.com/sammycage/lunasvg.git
        GIT_TAG        2246b6a171bb4e64a20d793861de8f44862fed53
        FIND_PACKAGE_ARGS
    )
    set(FMT_INSTALL OFF)
    set(FMT_TEST OFF)
    FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG        11.0.2
        FIND_PACKAGE_ARGS
    )
    set(JSON_BuildTests OFF)
    set(JSON_Install OFF)
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.11.3
        FIND_PACKAGE_ARGS
    )
    if(BUILD_TESTING)
        set(CATCH_INSTALL_DOCS OFF)
        FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.7.1
            FIND_PACKAGE_ARGS
        )
    endif()
    FetchContent_MakeAvailable(lunasvg fmt nlohmann_json)
endmacro()

macro(declare_photobox_dependencies)
    set(LUNASVG_BUILD_EXAMPLES OFF)
    FetchContent_Declare(
        lunasvg
        GIT_REPOSITORY https://github.com/sammycage/lunasvg.git
        GIT_TAG        23578c7461634623a5de903370310b3af5140536
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

    set(CPPTRACE_UNWIND_WITH_LIBUNWIND ON)
    FetchContent_Declare(
        cpptrace
        GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
        GIT_TAG        v0.7.2
        FIND_PACKAGE_ARGS
    )

    set(STDEXEC_BUILD_TESTS OFF)
    set(STDEXEC_BUILD_EXAMPLES OFF)
    FetchContent_Declare(
        stdexec
        GIT_REPOSITORY https://github.com/NVIDIA/stdexec.git
        GIT_TAG        c1508b1e46d5806a1c452c9c90821261716d1473
        FIND_PACKAGE_ARGS
    )

    set(QUILL_DISABLE_NON_PREFIXED_MACROS OFF)
    FetchContent_Declare(
        quill
        GIT_REPOSITORY https://github.com/odygrd/quill.git
        GIT_TAG        v7.4.0
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
        FetchContent_MakeAvailable(Catch2)
    endif()
    FetchContent_MakeAvailable(lunasvg fmt nlohmann_json cpptrace stdexec quill)
endmacro()

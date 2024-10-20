cmake_minimum_required(VERSION 3.28)
project(photobox VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")

include(CTest)
include(FetchContent)
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
set(STDEXEC_BUILD_TESTS OFF)
set(STDEXEC_BUILD_EXAMPLES OFF)
FetchContent_Declare(
  stdexec
  GIT_REPOSITORY https://github.com/NVIDIA/stdexec.git
  GIT_TAG        4d8d1944126fbc17d15894341f92838ef5db66cc
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
FetchContent_MakeAvailable(stdexec lunasvg fmt nlohmann_json)


find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Network Multimedia PrintSupport REQUIRED)
qt_policy(SET QTP0001 NEW)
qt_standard_project_setup()

find_package(fmt CONFIG REQUIRED)
find_package(lunasvg CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(stdexec CONFIG REQUIRED)
pkg_check_modules(gphoto2 REQUIRED IMPORTED_TARGET libgphoto2)
message(STATUS "Found gphoto2 ${gphoto2_VERSION}")

add_subdirectory(Photobox)

if(BUILD_TESTING)
    FetchContent_MakeAvailable(Catch2)
    find_package(Catch2 CONFIG REQUIRED)
    list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras")

    include(Catch)
    add_subdirectory(Tests)
endif()

install(PROGRAMS launcher.sh TYPE BIN)

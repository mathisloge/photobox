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
FetchContent_Declare(
  lunasvg
  GIT_REPOSITORY https://github.com/sammycage/lunasvg.git
  GIT_TAG        3769b62f762414f236a4886d08cb1d50ae1e5b36
  FIND_PACKAGE_ARGS
)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        11.0.2
  FIND_PACKAGE_ARGS
)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
    FIND_PACKAGE_ARGS
)
if(BUILD_TESTING)
    FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.7.1
    FIND_PACKAGE_ARGS
    )
endif()
FetchContent_MakeAvailable(lunasvg fmt nlohmann_json)


find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Network Multimedia PrintSupport REQUIRED)
qt_policy(SET QTP0001 NEW)
qt_standard_project_setup()

find_package(fmt CONFIG REQUIRED)
find_package(lunasvg CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)
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

set(CPACK_PACKAGE_CONTACT "Mathis")
set(CPACK_PACKAGE_FILE_NAME "photobox")
set(CPACK_RPM_SPEC_MORE_DEFINE "%define _build_id_links none")
include(CPack)

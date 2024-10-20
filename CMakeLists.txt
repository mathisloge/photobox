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
include(Dependencies)

declare_photobox_dependencies()

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

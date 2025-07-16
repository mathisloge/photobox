cmake_minimum_required(VERSION 3.30)
project(photobox VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/runtime")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
include(CPM)
include(CTest)
include(GenerateExportHeader)
include(Options)

declare_photobox_options()
CPMAddPackage(URI "gh:sammycage/lunasvg@3.3.0" OPTIONS "LUNASVG_BUILD_EXAMPLES OFF")
CPMAddPackage("gh:fmtlib/fmt#11.2.0")
CPMAddPackage(NAME "nlohmann_json" VERSION 3.12.0 GITHUB_REPOSITORY "nlohmann/json")
CPMAddPackage(URI "gh:jeremy-rifkin/cpptrace@1.0.3" OPTIONS "CPPTRACE_UNWIND_WITH_LIBUNWIND ON")
CPMAddPackage(URI "gh:NVIDIA/stdexec#2674bebb8389ce2faaadbea9c1d230b98ee56770" OPTIONS "STDEXEC_BUILD_TESTS OFF" "STDEXEC_BUILD_EXAMPLES OFF")
CPMAddPackage(URI "gh:odygrd/quill@10.0.1" OPTIONS "QUILL_DISABLE_NON_PREFIXED_MACROS OFF")

if(BUILD_TESTING)
    CPMAddPackage("gh:catchorg/Catch2@3.8.1")
endif()

find_package(Qt6 6.9 REQUIRED COMPONENTS Core Gui Qml Quick Network Multimedia PrintSupport)
qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0003 NEW)
qt_policy(SET QTP0004 NEW)
qt_policy(SET QTP0005 NEW)
qt_standard_project_setup()

find_package(PkgConfig REQUIRED)
pkg_check_modules(gphoto2 REQUIRED IMPORTED_TARGET libgphoto2)
message(STATUS "Found gphoto2 ${gphoto2_VERSION}")

add_subdirectory(Photobox)

if(BUILD_TESTING)
    add_subdirectory(Tests)
endif()

install(PROGRAMS launcher.sh TYPE BIN)

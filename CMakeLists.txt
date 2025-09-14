# SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
#
# SPDX-License-Identifier: GPL-3.0-or-later

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

set(BOOST_VERSION "1.89.0")
list(APPEND BOOST_INCLUDE_LIBRARIES asio)
cpmaddpackage(
  NAME Boost
  VERSION ${BOOST_VERSION}
  URL https://github.com/boostorg/boost/releases/download/boost-${BOOST_VERSION}/boost-${BOOST_VERSION}-cmake.tar.xz
  URL_HASH SHA256=67acec02d0d118b5de9eb441f5fb707b3a1cdd884be00ca24b9a73c995511f74
  OPTIONS "BOOST_ENABLE_CMAKE ON" "BOOST_SKIP_INSTALL_RULES ${CMAKE_SKIP_INSTALL_RULES}"
  FIND_PACKAGE_ARGUMENTS "GLOBAL CONFIG REQUIRED NAMES Boost-${BOOST_VERSION} CONFIGS BoostConfig.cmake"
  EXCLUDE_FROM_ALL ON
  SYSTEM ON
)
cpmaddpackage(URI "gh:sammycage/lunasvg@3.5.0" OPTIONS "LUNASVG_BUILD_EXAMPLES OFF")
cpmaddpackage("gh:fmtlib/fmt#11.2.0")
cpmaddpackage(NAME "nlohmann_json" VERSION 3.12.0 GITHUB_REPOSITORY "nlohmann/json")
cpmaddpackage(URI "gh:jeremy-rifkin/cpptrace@1.0.4" VERSION 1.0.4 OPTIONS "CPPTRACE_UNWIND_WITH_LIBUNWIND ON" FIND_PACKAGE_ARGUMENTS "CONFIG")
cpmaddpackage(URI "gh:NVIDIA/stdexec#daf12bcc46438f88b3c78212363f7b6531338780" VERSION 0.9.0 OPTIONS "CPM_LOCAL_PACKAGES ON" "STDEXEC_BUILD_TESTS OFF" "STDEXEC_BUILD_EXAMPLES OFF" "STDEXEC_ENABLE_ASIO ON" "STDEXEC_ASIO_IMPLEMENTATION boost" FIND_PACKAGE_ARGUMENTS "CONFIG"
)
cpmaddpackage(URI "gh:odygrd/quill@10.1.0" OPTIONS "QUILL_DISABLE_NON_PREFIXED_MACROS OFF")

if(BUILD_TESTING)
    cpmaddpackage(URI "gh:catchorg/Catch2@3.10.0" FIND_PACKAGE_ARGUMENTS "CONFIG")
    list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras") # local builds
    list(APPEND CMAKE_MODULE_PATH "${Catch2_DIR}") # flatpak builds
    include(Catch)
    find_package(Qt6 REQUIRED COMPONENTS Test)
endif()

find_package(
    Qt6
    6.9
    REQUIRED
    COMPONENTS Core Gui Qml Quick QuickControls2 Network Multimedia PrintSupport
)
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
    #add_subdirectory(Tests)
endif()

install(PROGRAMS launcher.sh TYPE BIN)

cmake_minimum_required(VERSION 3.28)
project(photobox VERSION 0.0.1 LANGUAGES CXX)

include(CTest)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Network Multimedia PrintSupport REQUIRED)
qt_policy(SET QTP0001 NEW)

qt_standard_project_setup()

find_package(PkgConfig REQUIRED)
pkg_check_modules(gphoto2 REQUIRED IMPORTED_TARGET libgphoto2)
message(STATUS "Found gphoto2 ${gphoto2_VERSION}")

add_subdirectory(Photobox)

if(BUILD_TESTING)
    add_subdirectory(Tests)
endif()

set(CPACK_PACKAGE_CONTACT "Mathis")
set(CPACK_PACKAGE_FILE_NAME "photobox")
set(CPACK_RPM_SPEC_MORE_DEFINE "%define _build_id_links none")
include(CPack)

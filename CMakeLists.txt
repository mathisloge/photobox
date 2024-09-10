cmake_minimum_required(VERSION 3.30)
project(photobox VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "/home/mlogemann/Qt/6.7.2/gcc_64")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Network Multimedia REQUIRED)
qt_policy(SET QTP0001 NEW)

qt_standard_project_setup()

find_package(PkgConfig REQUIRED)
pkg_check_modules(gphoto2 REQUIRED IMPORTED_TARGET libgphoto2)
message(STATUS "Found gphoto2 ${gphoto2_VERSION}")

add_subdirectory(Photobox)

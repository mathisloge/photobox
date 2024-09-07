cmake_minimum_required(VERSION 3.30)
project(photobox VERSION 0.0.1 LANGUAGES CXX)



find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick Network Multimedia REQUIRED)
qt_policy(SET QTP0001 NEW)


qt_standard_project_setup()

add_subdirectory(Photobox)

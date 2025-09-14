# SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
#
# SPDX-License-Identifier: GPL-3.0-or-later

add_library(PhotoboxGPhoto2 STATIC)
add_library(Photobox::GPhoto2 ALIAS PhotoboxGPhoto2)
target_link_libraries(
    PhotoboxGPhoto2
    PUBLIC Qt6::Gui Photobox::Logging STDEXEC::stdexec PkgConfig::gphoto2
)

target_include_directories(PhotoboxGPhoto2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(
    PhotoboxGPhoto2
    PRIVATE
        GPhoto2Context.cpp
        GPhoto2Context.hpp
        GPhoto2Wrapper.hpp
        GPhoto2Wrapper.cpp
        GPhoto2Integration.hpp
        GPhoto2Integration.cpp
        GPhoto2Exeption.hpp
)

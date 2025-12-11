# SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
#
# SPDX-License-Identifier: GPL-3.0-or-later

add_library(PhotoboxGPhoto2 STATIC)
add_library(Photobox::GPhoto2 ALIAS PhotoboxGPhoto2)
target_link_libraries(PhotoboxGPhoto2 PUBLIC Photobox::Core)
target_sources(
    PhotoboxGPhoto2
    PUBLIC
        FILE_SET headers
            TYPE HEADERS
            BASE_DIRS Include
            FILES
                Include/Pbox/GPhoto2Camera.hpp
                Include/Pbox/GPhoto2Exeption.hpp
    PRIVATE
        Source/GPhoto2Camera.cpp
        Source/GPhoto2Context.cpp
        Source/GPhoto2Context.hpp
        Source/GPhoto2Integration.cpp
        Source/GPhoto2Integration.hpp
        Source/GPhoto2Wrapper.cpp
        Source/GPhoto2Wrapper.hpp
)
target_link_libraries(
    PhotoboxGPhoto2
    PUBLIC
        Photobox::Logging
        Boost::headers
        Boost::asio
        Boost::system
        STDEXEC::stdexec
        Photobox::Core
        PkgConfig::gphoto2
)

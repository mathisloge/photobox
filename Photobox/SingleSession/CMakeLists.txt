# SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
#
# SPDX-License-Identifier: GPL-3.0-or-later

add_library(PhotoboxSingleSession STATIC)
add_library(Photobox::SingleSession ALIAS PhotoboxSingleSession)
target_link_libraries(PhotoboxSingleSession PUBLIC Photobox::Core)
target_sources(
    PhotoboxSingleSession
    PUBLIC
        FILE_SET headers
            TYPE HEADERS
            BASE_DIRS Include
            FILES Include/Pbox/Sessions/SingleCaptureSessionFactory.hpp
    PRIVATE
        Source/SingleCaptureSessionFactory.cpp
        Source/SingleCaptureSession.hpp
        Source/SingleCaptureSession.cpp
)

if(BUILD_TESTING)
    #add_subdirectory(Tests)
endif()

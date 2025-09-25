# SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
#
# SPDX-License-Identifier: GPL-3.0-or-later

add_library(PhotoboxCollageSession STATIC)
add_library(Photobox::CollageSession ALIAS PhotoboxCollageSession)
target_link_libraries(
    PhotoboxCollageSession
    PUBLIC Photobox::Core lunasvg::lunasvg
)
target_sources(
    PhotoboxCollageSession
    PUBLIC
        FILE_SET headers
            TYPE HEADERS
            BASE_DIRS Include
            FILES
                Include/Pbox/Sessions/CollageCaptureSessionFactory.hpp
                Include/Pbox/Sessions/CollageRenderer.hpp
    PRIVATE
        Source/CollageCaptureSession.cpp
        Source/CollageCaptureSession.hpp
        Source/CollageCaptureSessionFactory.cpp
        Source/CollageRenderer.cpp
)

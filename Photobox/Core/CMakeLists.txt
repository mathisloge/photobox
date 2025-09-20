# SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
#
# SPDX-License-Identifier: GPL-3.0-or-later

qt_add_qml_module(
    PhotoboxCore
    URI "Photobox.Core"
    STATIC
    SOURCES
        ICamera.hpp
        ICamera.cpp
        MockCamera.hpp
        MockCamera.cpp
        ImageStorage.hpp
        ImageStorage.cpp
        ImageProvider.hpp
        ImageProvider.cpp
        ApplicationState.hpp
        CollageRenderer.hpp
        CollageRenderer.cpp
        CollageSettings.hpp
        CollagePrinter.hpp
        CollagePrinter.cpp
        CollageFontCache.hpp
        CollageFontCache.cpp
        CollageCaptureSession.hpp
        CollageCaptureSession.cpp
        CollageCaptureSessionFactory.hpp
        CollageCaptureSessionFactory.cpp
        CaptureSessionFactory.hpp
        CaptureSessionFactory.cpp
        RemoteTrigger.cpp
        RemoteTrigger.hpp
        CameraLed.cpp
        CameraLed.hpp
        CrashCollection.cpp
        CrashCollection.hpp
        GPhoto2Camera.hpp
        GPhoto2Camera.cpp
        Scheduler.cpp
        Scheduler.hpp
        ICaptureSession.cpp
        ICaptureSession.hpp
        SingleCaptureSession.hpp
        SingleCaptureSession.cpp
        IdleCaptureSession.hpp
        IdleCaptureSession.cpp
        CaptureManager.hpp
        CaptureManager.cpp
        SystemStatusCode.hpp
        SystemStatusManager.hpp
        SystemStatusManager.cpp
        SystemStatusModel.cpp
        SystemStatusModel.hpp
        SystemStatusClient.hpp
        SystemStatusClient.cpp
        TriggerManager.hpp
        TriggerManager.cpp
        Countdown.hpp
        Countdown.cpp
    RESOURCES
        Fonts/GreatVibes/GreatVibes-Regular.ttf
        Fonts/Roboto/Roboto-Regular.ttf
)

add_library(Photobox::Core ALIAS PhotoboxCore)
add_library(Photobox::CorePlugin ALIAS PhotoboxCoreplugin)

target_include_directories(PhotoboxCore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(
    PhotoboxCore
    PUBLIC
        Qt6::Quick
        Qt6::PrintSupport
        Qt6::Multimedia
        Boost::headers
        Photobox::Utils
        Photobox::GPhoto2
        cpptrace::cpptrace
        lunasvg::lunasvg
        nlohmann_json::nlohmann_json
        fmt::fmt
)

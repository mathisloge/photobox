# SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
#
# SPDX-License-Identifier: GPL-3.0-or-later

qt_add_qml_module(
    PhotoboxSettings
    URI "Photobox.Settings"
    STATIC
    QML_FILES
        Qml/PrinterSettingsPage.qml
        Qml/ProjectSettingsPage.qml
)
add_library(Photobox::Settings ALIAS PhotoboxSettings)
add_library(Photobox::SettingsPlugin ALIAS PhotoboxSettingsplugin)

target_sources(
    PhotoboxSettings
    PUBLIC
        FILE_SET headers
            TYPE HEADERS
            BASE_DIRS Include
            FILES
                Include/Pbox/Settings/Types.hpp
                Include/Pbox/Settings/Project.hpp
                Include/Pbox/Settings/Detail/TypesSerialization.hpp
    PRIVATE Source/Project.cpp
)

target_link_libraries(
    PhotoboxSettings
    PUBLIC
        nlohmann_json::nlohmann_json
        Photobox::Core
        Photobox::Peripheral
        Photobox::CollageSession
        Photobox::SingleSession
)

if(BUILD_TESTING)
    add_subdirectory(Tests)
endif()

# SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
#
# SPDX-License-Identifier: GPL-3.0-or-later

qt_add_executable(PhotoboxApp main.cpp)

target_link_libraries(
    PhotoboxApp
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::PrintSupport
        Photobox::Core
        Photobox::CorePlugin
        Photobox::SettingsPlugin
        PhotoboxUiplugin
        PhotoboxPeripheral
)

qt_add_qml_module(PhotoboxApp
    URI "Photobox.App"
    VERSION 1.0
    QML_FILES
      Main.qml
    DEPENDENCIES
        TARGET Photobox::Core
        TARGET Photobox::Ui
)

install(
    TARGETS PhotoboxApp
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

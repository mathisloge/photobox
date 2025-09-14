# SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
#
# SPDX-License-Identifier: GPL-3.0-or-later

qt_add_qml_module(
    PhotoboxUi
    URI "Photobox.Ui"
    STATIC
    QML_FILES
        Countdown.qml
        PreviewImage.qml
        BusyPage.qml
        SystemStatusDelegate.qml
        StatusPage.qml
        CaptureMainPage.qml
        CapturePage.qml
        PreviewPage.qml
        BuzzerAnimation.qml
    RESOURCES
        Buzzer.svg
        Pointer.svg
        ImageBorder.svg
        Logo.svg
    DEPENDENCIES
        TARGET Photobox::Core
)
add_library(Photobox::Ui ALIAS PhotoboxUi)

target_include_directories(PhotoboxUi PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(PhotoboxUi PUBLIC Qt6::Quick)

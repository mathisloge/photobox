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
        CapturePage.qml
    RESOURCES
        ImageBorder.svg
        Logo.svg
)

target_include_directories(PhotoboxUi PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(PhotoboxUi PUBLIC
    Qt6::Quick
)

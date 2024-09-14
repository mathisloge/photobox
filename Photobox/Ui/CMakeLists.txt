qt_add_qml_module(
    PhotoboxUi
    URI "Photobox.Ui"
    STATIC
    QML_FILES
        Countdown.qml
        PreviewImage.qml
        ShutterEffect.qml
        ShutterShader.qml
    RESOURCES
        ImageBorder.svg
        ShutterShader/shuttershader.vert.qsb
        ShutterShader/shuttershader.frag.qsb
)

target_include_directories(PhotoboxUi PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(PhotoboxUi PUBLIC
    Qt6::Quick
)

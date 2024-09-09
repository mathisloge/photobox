qt_add_executable(PhotoboxApp main.cpp)

target_link_libraries(PhotoboxApp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    PhotoboxCore
    PhotoboxCoreplugin
)

qt_add_qml_module(PhotoboxApp
    URI "Photobox.App"
    VERSION 1.0
    QML_FILES
      Main.qml
      Countdown.qml
      ShutterEffect.qml
      ShutterShader.qml
    RESOURCES
        ShutterShader/shuttershader.vert.qsb
        ShutterShader/shuttershader.frag.qsb
        camera-shutter.wav
        border.svg
)


install(TARGETS PhotoboxApp
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_qml_app_script(
    TARGET PhotoboxApp
    OUTPUT_SCRIPT deploy_script
    MACOS_BUNDLE_POST_BUILD
    NO_UNSUPPORTED_PLATFORM_ERROR
    DEPLOY_USER_QML_MODULES_ON_UNSUPPORTED_PLATFORM
)
install(SCRIPT ${deploy_script})

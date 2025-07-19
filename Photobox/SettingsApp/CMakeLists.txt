qt_add_executable(PhotoboxSettingsApp main.cpp)

target_link_libraries(PhotoboxSettingsApp PRIVATE
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

qt_add_qml_module(PhotoboxSettingsApp
    URI "Photobox.SettingsApp"
    VERSION 1.0
    QML_FILES
      Main.qml
    DEPENDENCIES
        Photobox::Core
        Photobox::Settings
)


install(TARGETS PhotoboxSettingsApp
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

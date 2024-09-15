qt_add_executable(PhotoboxApp main.cpp)

target_link_libraries(PhotoboxApp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    PhotoboxCore
    PhotoboxCoreplugin
    PhotoboxGPhoto2
    PhotoboxGPhoto2plugin
    PhotoboxUiplugin
)

qt_add_qml_module(PhotoboxApp
    URI "Photobox.App"
    VERSION 1.0
    QML_FILES
      Main.qml
)


install(TARGETS PhotoboxApp
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

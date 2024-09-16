qt_add_executable(CollageEditorApp main.cpp)

target_link_libraries(CollageEditorApp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::PrintSupport
    PhotoboxCore
    PhotoboxCoreplugin
    PhotoboxGPhoto2
    PhotoboxGPhoto2plugin
    PhotoboxUiplugin
)

qt_add_qml_module(CollageEditorApp
    URI "Photobox.CollageEditorApp"
    VERSION 1.0
    QML_FILES
      Main.qml
)


install(TARGETS CollageEditorApp
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

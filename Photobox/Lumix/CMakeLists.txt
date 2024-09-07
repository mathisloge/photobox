qt_add_qml_module(
    PhotoboxLumix
    URI "Photobox.Lumix"
    STATIC
    QML_FILES
        LumixCamera.qml
    SOURCES
        Backend/CameraNetwork.hpp
        Backend/CameraNetwork.cpp
)
target_include_directories(PhotoboxLumix PRIVATE Backend)

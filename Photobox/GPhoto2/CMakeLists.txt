qt_add_qml_module(
    PhotoboxGPhoto2
    URI "Photobox.GPhoto2"
    STATIC
    SOURCES
        GPhoto2Camera.hpp
        GPhoto2Camera.cpp
)
target_link_libraries(PhotoboxGPhoto2 PUBLIC 
    PhotoboxCore
    PkgConfig::gphoto2
)

target_include_directories(PhotoboxGPhoto2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
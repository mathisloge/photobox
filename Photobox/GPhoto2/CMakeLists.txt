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

target_sources(PhotoboxGPhoto2 PRIVATE
    GPhoto2CameraImpl.cpp
    GPhoto2CameraImpl.hpp
    GPhoto2Context.cpp
    GPhoto2Context.hpp
    GPhoto2List.cpp
    GPhoto2List.hpp
    GPhoto2Worker.cpp
    GPhoto2Worker.hpp
)

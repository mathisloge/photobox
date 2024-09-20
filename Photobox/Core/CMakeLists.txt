qt_add_qml_module(
    PhotoboxCore
    URI "Photobox.Core"
    STATIC
    SOURCES
        CameraImageProvider.hpp
        CameraImageProvider.cpp
        ICamera.hpp
        ICamera.cpp
        MockCamera.hpp
        MockCamera.cpp
        ImageStorage.hpp
        ImageStorage.cpp
        ApplicationState.hpp
        PhotoTriggerClient.cpp
        PhotoTriggerClient.hpp
        CollageRenderer.hpp
        CollageRenderer.cpp
)

target_include_directories(PhotoboxCore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(PhotoboxCore PUBLIC
    Qt6::Quick
    Qt6::Multimedia
    lunasvg
    nlohmann_json::nlohmann_json
)

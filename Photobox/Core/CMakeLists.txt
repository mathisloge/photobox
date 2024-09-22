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
        CaptureController.hpp
        CaptureController.cpp
        CollageSettings.hpp
        CollagePrinter.hpp
        CollagePrinter.cpp
    RESOURCES
        Fonts/GreatVibes/GreatVibes-Regular.ttf
        Fonts/Roboto/Roboto-Bold.ttf
        Fonts/Roboto/Roboto-BoldItalic.ttf
        Fonts/Roboto/Roboto-Italic.ttf
        Fonts/Roboto/Roboto-Regular.ttf
)

target_include_directories(PhotoboxCore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(PhotoboxCore PUBLIC
    Qt6::Quick
    Qt6::PrintSupport
    Qt6::Multimedia
    lunasvg
    nlohmann_json::nlohmann_json
    fmt::fmt
)

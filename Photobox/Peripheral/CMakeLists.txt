add_library(PhotoboxPeripheral STATIC)

target_sources(PhotoboxPeripheral PRIVATE
    EspHomeClient.cpp
    EspHomeClient.hpp
    EspHomeRemoteTrigger.cpp
    EspHomeRemoteTrigger.hpp
    IEspHomeClient.hpp
)

target_include_directories(PhotoboxPeripheral PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(PhotoboxPeripheral PUBLIC
    Qt6::Network
    nlohmann_json::nlohmann_json
    fmt::fmt
    PhotoboxCore
)

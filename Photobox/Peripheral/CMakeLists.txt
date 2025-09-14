# SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
#
# SPDX-License-Identifier: GPL-3.0-or-later

add_library(PhotoboxPeripheral STATIC)
add_library(Photobox::Peripheral ALIAS PhotoboxPeripheral)
target_sources(
    PhotoboxPeripheral
    PRIVATE
        EspHomeClient.cpp
        EspHomeClient.hpp
        EspHomeRemoteTrigger.cpp
        EspHomeRemoteTrigger.hpp
        EspHomeCameraLed.cpp
        EspHomeCameraLed.hpp
        IEspHomeClient.hpp
)

target_include_directories(
    PhotoboxPeripheral
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries(
    PhotoboxPeripheral
    PUBLIC Qt6::Network nlohmann_json::nlohmann_json fmt::fmt PhotoboxCore
)

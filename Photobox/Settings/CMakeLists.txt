qt_add_qml_module(
    PhotoboxSettings
    URI "Photobox.Settings"
    STATIC
    SOURCES

)
add_library(Photobox::Settings ALIAS PhotoboxSettings)
add_library(Photobox::SettingsPlugin ALIAS PhotoboxSettingsplugin)

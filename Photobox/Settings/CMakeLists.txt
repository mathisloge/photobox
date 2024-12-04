qt_add_qml_module(
    PhotoboxSettings
    URI "Photobox.Settings"
    STATIC
    QML_FILES
        PrinterSettingsPage.qml
        ProjectSettingsPage.qml
)
add_library(Photobox::Settings ALIAS PhotoboxSettings)
add_library(Photobox::SettingsPlugin ALIAS PhotoboxSettingsplugin)

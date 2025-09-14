// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import Photobox.Core
import Photobox.Ui
import Photobox.Settings
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window

    property bool showSettings: false

    header: ToolBar {
        visible: ApplicationState.systemStatusManager.status === SystemStatusManager.Initializing
        RowLayout {
            anchors.fill: parent
            ToolButton {
                text: window.showSettings ? qsTr("Close") : qsTr("Settings")
                onClicked: window.showSettings = !window.showSettings
            }
        }
    }

    Loader {
        id: viewLoader

        anchors.fill: parent
        sourceComponent: statusPage

        states: [
            State {
                name: "status"
                when: !window.showSettings && ApplicationState.systemStatusManager.status === SystemStatusManager.Initializing
                PropertyChanges {
                    viewLoader.sourceComponent: statusPage
                }
            },
            State {
                name: "normal"
                when: !window.showSettings && ApplicationState.systemStatusManager.status === SystemStatusManager.RequiredClientsInitilized || ApplicationState.systemStatusManager.status === SystemStatusManager.AllClientsInitializied
                PropertyChanges {
                    viewLoader.sourceComponent: capturePage
                }
            },
            State {
                name: "settings"
                when: window.showSettings
                PropertyChanges {
                    viewLoader.sourceComponent: settingsPage
                }
            }
        ]
    }

    Component {
        id: statusPage

        StatusPage {}
    }

    Component {
        id: capturePage

        CaptureMainPage {}
    }

    Component {
        id: settingsPage

        SplitView {
            ScrollView {
                SplitView.preferredWidth: 80
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

                ColumnLayout {
                    width: parent.width

                    Button {
                        Layout.fillWidth: true
                        text: "Project"
                    }
                }
            }

            ProjectSettingsPage {
                SplitView.fillWidth: true
            }
        }
    }
}

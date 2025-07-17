// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathis.opensource@tuta.io>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import Photobox.Core
import Photobox.Ui
import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: window

    Loader {
        id: viewLoader

        anchors.fill: parent
        sourceComponent: statusPage

        states: [
            State {
                name: "status"
                when: ApplicationState.systemStatusManager.status === SystemStatusManager.Initializing
                PropertyChanges {
                    viewLoader.sourceComponent: statusPage
                }
            },
            State {
                name: "normal"
                when: ApplicationState.systemStatusManager.status === SystemStatusManager.RequiredClientsInitilized || ApplicationState.systemStatusManager.status === SystemStatusManager.AllClientsInitializied
                PropertyChanges {
                    viewLoader.sourceComponent: capturePage
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
}

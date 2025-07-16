// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import Photobox.Core

Page {
    id: root
    readonly property ICaptureSession session: ApplicationState.captureManager.session

    StackView {
        id: stack
        initialItem: capturePage
        anchors.fill: parent

        pushEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: 500
            }
        }

        pushExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: 500
            }
        }

        popEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: 500
            }
        }

        popExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: 500
            }
        }
    }

    states: [
        State {
            name: "capturing"
            when: root.session.status !== ICaptureSession.Busy && !root.session.previewImage
            StateChangeScript {
                script: stack.pop(null)
            }
        },
        State {
            name: "busy"
            when: root.session.status === ICaptureSession.Busy
            StateChangeScript {
                script: stack.push(busyPage)
            }
        },
        State {
            name: "preview"
            when: root.session.status !== ICaptureSession.Busy && root.session.previewImage
            StateChangeScript {
                script: stack.push(previewPage)
            }
        }
    ]

    Component {
        id: capturePage
        CapturePage {}
    }

    Component {
        id: busyPage
        BusyPage {}
    }

    Component {
        id: previewPage
        PreviewPage {
            imageSource: root.session.previewImage
        }
    }
}

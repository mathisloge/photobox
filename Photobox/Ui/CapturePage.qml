// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later
pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Photobox.Core

Page {
    id: root
    readonly property ICaptureSession session: ApplicationState.captureManager.session
    background: PageBackground {}
    VideoBox {
        id: videoOutput

        anchors.fill: parent
        anchors.margins: 20

        opacity: root.session.liveViewVisible ? 1 : 0

        Component.onCompleted: {
            ApplicationState.captureManager.camera.videoSink = videoOutput.videoSink;
        }
    }

    Countdown {
        id: countdown

        anchors.centerIn: parent
        opacity: root.session.countdown.visible ? 1 : 0
        text: root.session.countdown.text
    }

    RowLayout {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        Repeater {
            id: list
            model: CaptureSessionList {
                sessionManager: ApplicationState.captureSessionManager
            }

            delegate: Buzzer {
                required property string sessionId
                required property string name
                text: name
                opacity: (!root.session.liveViewVisible || root.session.countdown.visible) ? 0 : 1
                onClicked: {
                    ApplicationState.captureManager.sessionButtonPressed(sessionId);
                }
            }
        }
    }
}

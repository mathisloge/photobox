// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathis.opensource@tuta.io>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtMultimedia
import Photobox.Core

Page {
    id: root
    readonly property ICaptureSession session: ApplicationState.captureManager.session

    VideoOutput {
        id: videoOutput

        anchors.fill: parent

        opacity: root.session.liveViewVisible ? 1 : 0

        Behavior on opacity {
            NumberAnimation {
                duration: 500
            }
        }

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

    BuzzerAnimation {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        opacity: root.session.status === ICaptureSession.Idle ? 1 : 0
    }

    MouseArea {
        id: manualCaptureArea
        anchors.fill: parent
        onClicked: {
            ApplicationState.captureManager.triggerButtonPressed();
        }
    }
}

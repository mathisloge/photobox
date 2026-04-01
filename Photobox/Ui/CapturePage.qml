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

    ListView {
        id: list
        model: ListModel {
            ListElement {
                name: "Einzelfoto"
                buzzColor: "#FF00FF"
            }
            ListElement {
                name: "Mehrfachfoto"
                buzzColor: "#FFD700"
            }
            ListElement {
                name: "Collage"
                buzzColor: "#6200EA"
            }
        }
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.left: parent.left
        anchors.right: parent.right
        height: 240
        orientation: ListView.Horizontal

        delegate: Buzzer {
            required property string name
            required property color buzzColor
            color: buzzColor
            text: name
            onClicked: {
                ApplicationState.captureManager.triggerButtonPressed("Display");
            }
        }
    }
}

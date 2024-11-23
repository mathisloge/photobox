import QtMultimedia
import QtQuick
import QtQuick.Controls
import Photobox.Core

Page {
    id: root
    property alias videoOutput: videoOutput
    property alias countdownText: countdown.text
    property alias countdownVisible: countdown.visible

    readonly property ICaptureSession session: ApplicationState.captureManager.session

    VideoOutput {
        id: videoOutput

        anchors.fill: parent

        opacity: root.session.previewVisible ? 1 : 0

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
        opacity: root.session.countdownVisible ? 1 : 0
        text: root.session.countdownText
    }

    BuzzerAnimation {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        opacity: root.session.idle ? 1 : 0
    }

    MouseArea {
        id: manualCaptureArea
        anchors.fill: parent
        onClicked: {
            ApplicationState.captureManager.triggerButtonPressed();
        }
    }
}

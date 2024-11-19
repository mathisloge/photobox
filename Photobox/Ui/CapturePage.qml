import QtMultimedia
import QtQuick
import QtQuick.Controls

Page {
    property alias videoOutput: videoOutput
    property alias countdownText: countdown.text
    property alias countdownVisible: countdown.visible

    VideoOutput {
        id: videoOutput

        anchors.fill: parent

        Behavior on opacity {
            NumberAnimation {
                duration: 500
            }
        }
    }

    Countdown {
        id: countdown

        anchors.fill: parent
    }
}

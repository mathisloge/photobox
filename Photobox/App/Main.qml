import QtQuick
import QtQuick.Controls
import Photobox.Lumix

ApplicationWindow {
    visible: true

    //LumixCamera {
    //    anchors.fill: parent
    //}
    Button {
        text: "start"
        visible: !shutter.running
        onClicked: shutter.running = true
    }
    Countdown {
        id: countdown
        anchors.fill: parent
        visible: running
    }

    ShutterEffect {
        id: shutter
        anchors.fill: parent
    }
}

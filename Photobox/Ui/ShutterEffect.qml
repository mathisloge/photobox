import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

Item {
    id: root

    property alias running: effect.timeRunning

    ShutterShader {
        id: effect

        anchors.fill: parent
    }

}

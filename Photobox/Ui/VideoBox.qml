pragma ComponentBehavior: Bound
import QtQuick
import QtMultimedia
import QtQuick.Effects

Item {
    id: root
    property alias videoSink: videoOutput.videoSink

    readonly property real radius: 12

    implicitHeight: videoOutput.height
    implicitWidth: videoOutput.width

    Rectangle {
        id: glow
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        radius: root.radius
        layer.enabled: true
        layer.effect: GlowEffect {}
    }

    Rectangle {
        id: mask
        anchors.fill: parent
        radius: root.radius
        visible: false
        layer.enabled: true
    }

    Item {
        id: card
        anchors.fill: parent
        layer.enabled: true
        layer.effect: MultiEffect {
            maskEnabled: true
            maskSource: mask
        }

        VideoOutput {
            id: videoOutput

            width: root.width
            height: root.height

            fillMode: VideoOutput.PreserveAspectCrop
        }
    }

    Behavior on opacity {
        NumberAnimation {
            duration: 500
        }
    }
}

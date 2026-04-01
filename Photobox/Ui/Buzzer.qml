pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Effects

Item {
    id: root
    implicitWidth: 240
    implicitHeight: 240

    required property string text
    required property color color

    signal clicked

    // Outer bezel
    Rectangle {
        anchors.fill: parent
        radius: width / 2
        color: "#2A2A38"
        border.width: 2
        border.color: root.color
    }

    // Inner panel ring
    Rectangle {
        anchors {
            fill: parent
            margins: 12
        }
        radius: width / 2
        color: "#18181F"
        border.width: 1.5
        border.color: "#3A3A50"
    }

    // Ripple rings
    Repeater {
        model: 3
        delegate: Rectangle {
            id: ring
            required property int index
            property int idx: index
            anchors.centerIn: parent
            width: root.width
            height: root.height
            radius: width / 2
            color: "transparent"
            border.color: root.color
            border.width: 1.5
            opacity: 0
            scale: 1

            SequentialAnimation {
                id: anim
                PauseAnimation {
                    duration: ring.idx * 130
                }
                ParallelAnimation {
                    NumberAnimation {
                        target: ring
                        property: "scale"
                        from: 0.85
                        to: 1.65
                        duration: 720
                        easing.type: Easing.OutCubic
                    }
                    NumberAnimation {
                        target: ring
                        property: "opacity"
                        from: 0.60
                        to: 0.00
                        duration: 720
                        easing.type: Easing.OutCubic
                    }
                }
            }

            Connections {
                target: root
                function onClicked() {
                    anim.restart();
                }
            }
        }
    }

    // Glow halo
    Rectangle {
        id: halo
        anchors {
            fill: parent
            margins: 14
        }
        radius: width / 2
        color: "transparent"
        border.width: 12
        border.color: root.color
        layer.enabled: true
        layer.effect: MultiEffect {
            blurEnabled: true
            blur: 1.0
            blurMax: 20
        }
    }
    SequentialAnimation {
        running: true
        loops: Animation.Infinite
        NumberAnimation {
            target: halo
            property: "opacity"
            to: 0.85
            duration: 700
            easing.type: Easing.InOutSine
        }
        NumberAnimation {
            target: halo
            property: "opacity"
            to: 0.30
            duration: 700
            easing.type: Easing.InOutSine
        }
    }

    // Dome
    Rectangle {
        id: dome
        anchors {
            fill: parent
            margins: 24
        }
        radius: width / 2
        color: root.color
        scale: mouseArea.pressed ? 0.93 : 1.0
        Behavior on scale {
            NumberAnimation {
                duration: 100
                easing.type: Easing.OutQuad
            }
        }

        Text {
            id: domeLabel
            anchors {
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
                verticalCenterOffset: -8
            }
            text: root.text
            font {
                pixelSize: 18
                weight: Font.Black
                letterSpacing: 5
            }
            color: "#FFFFFFCC"
            opacity: mouseArea.pressed ? 0.4 : 1.0
            Behavior on opacity {
                NumberAnimation {
                    duration: 80
                }
            }
        }

        Text {
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: domeLabel.bottom
                topMargin: 4
            }
            text: "drücken"
            font {
                pixelSize: 9
                letterSpacing: 2
            }
            color: "#FFFFFF55"
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            root.clicked();
        }
    }
}

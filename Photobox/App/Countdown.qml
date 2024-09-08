import QtQuick
import QtQuick.Controls

Item {
    id: root

    property int initialCount: 10
    property alias running: countdownTimer.running
    property alias textColor: countdownText.color

    property int __count: 0

    signal finished()

    Timer {
        id: countdownTimer
        interval: 1000
        repeat: true
        onTriggered: {
            if (__count > 0) {
                __count--;
                // finish when the smile is beeing displayed
                if(__count === 0) {
                    root.finished();
                }
            } else {
                countdownTimer.stop();
            }
        }

        onRunningChanged: {
            if (this.running) {
                root.__count = root.initialCount;
            }
        }
    }

    Text {
        id: countdownText
        text: __count > 0 ? __count : "Smile!"
        font.pixelSize: 192
        color: "white"
        anchors.centerIn: parent
        scale: 1
        opacity: 1

        Behavior on text {
            SequentialAnimation {
                NumberAnimation {
                    target: countdownText
                    property: "scale"
                    from: 1
                    to: 0.5
                    duration: 800
                    easing.type: Easing.InOutQuad
                }

                NumberAnimation {
                    target: countdownText
                    property: "opacity"
                    from: 1
                    to: 0
                    duration: 200
                    easing.type: Easing.InQuad
                }
            }
        }
        onTextChanged: {
            countdownText.scale = 1;
            countdownText.opacity = 1;
        }
    }
}

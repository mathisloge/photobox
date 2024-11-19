import QtQuick
import QtQuick.Controls

Text {
    id: countdownText

    font.pixelSize: 192
    color: "white"
    anchors.centerIn: parent
    scale: 1
    opacity: 1
    onTextChanged: {
        countdownText.scale = 1;
        countdownText.opacity = 1;
    }

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
}

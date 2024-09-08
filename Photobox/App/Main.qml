import QtQuick
import QtQuick.Controls
import QtMultimedia
import Photobox.Lumix

ApplicationWindow {
    visible: true

    LumixCamera {
        anchors.fill: parent
    }


    SoundEffect {
        id: playSound
        source: "camera-shutter.wav"
    }

    ShutterEffect {
        id: shutter
        anchors.fill: parent
    }

    Countdown {
        id: countdown
        anchors.fill: parent
        visible: running
        initialCount: 5

        onFinished: {
            shutter.running = true
            playSound.play()
        }
    }


    Button {
        text: "start"
        visible: !countdown.running
        onClicked: countdown.running = true
    }

    
}

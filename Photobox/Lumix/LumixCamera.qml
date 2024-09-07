import QtQuick
import QtQuick.Controls
import QtMultimedia

Rectangle {
    CameraNetwork{
        id: network
    }
    MediaPlayer {
        id: player
        source: "udp://@:5111"
        videoOutput: videoOutput
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent
    }

    Text {
        anchors.centerIn: parent
        text: player.error + " " + player.errorString + "hasVideo"+player.hasVideo //"Battery:" + network.state.batteryRemaining + "%"
    }

    Button {
        anchors.top: parent.top
        text: player.playing ? "stop" : "play"
        onClicked: {
            if(player.playing) {
                player.stop()
            } else {
                player.source = "udp://@:5111"
                player.play()
            }
        }
    }
}
// http://192.168.54.1/cam.cgi?mode=startstream&value=5111
// http://192.168.54.1/cam.cgi?mode=stopstream

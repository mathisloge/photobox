// http://192.168.54.1/cam.cgi?mode=stopstream

import QtMultimedia
import QtQuick
import QtQuick.Controls

Rectangle {
    CameraNetwork {
        id: network
    }

    MediaPlayer {
        id: player

        source: "udp://@:5111?overrun_nonfatal=1&fifo_size=10000000"
        videoOutput: videoOutput
        playbackRate: 2
        autoPlay: true
    }

    VideoOutput {
        id: videoOutput

        anchors.fill: parent
    }

    Text {
        anchors.centerIn: parent
        text: player.error + " " + player.errorString + "hasVideo" + player.hasVideo + "Battery:" + network.state.batteryRemaining + "%"
    }

    Button {
        id: btnPlay

        anchors.top: parent.top
        text: player.playing ? "stop" : "play"
        onClicked: {
            if (player.playing) {
                player.stop();
            } else {
                player.position = 0;
                player.source = "udp://@:5111?overrun_nonfatal=1&fifo_size=10000000";
                player.play();
            }
        }
    }

    Button {
        anchors.top: btnPlay.bottom
        text: "Start"
        onClicked: {
            network.captureImage();
        }
    }

}
// http://192.168.54.1/cam.cgi?mode=startstream&value=5111

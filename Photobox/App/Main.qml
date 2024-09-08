import QtQuick
import QtQuick.Controls
import QtMultimedia
import Photobox.Core

ApplicationWindow {
    visible: true

    Loader { anchors.fill: parent; id: pageLoader; sourceComponent: liveFeedComponent }

    Component {
        id: liveFeedComponent

        Item {
            anchors.fill: parent
            VideoOutput {
                id: videoOutput
                anchors.fill: parent

                Component.onCompleted: {
                    ApplicationState.camera.videoSink = videoOutput.videoSink
                }
            }

            SoundEffect {
                id: shutterSound
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
                    ApplicationState.camera.requestCapturePhoto();
                    shutterSound.play()
                }
            }

            Button {
                text: "start"
                visible: !countdown.running
                onClicked: countdown.running = true
            }
        }
    }

    Component {
        id: test
        BorderImage {
    anchors { fill: parent; margins: 1 }
    border { left: 30; top: 30; right: 30; bottom: 30 }
        horizontalTileMode: BorderImage.Stretch
    verticalTileMode: BorderImage.Stretch
            source: "image://camera/capture" 
        }
    }

    Timer {
        id: timer
        interval: 0; running: false; repeat: false
        onTriggered: pageLoader.sourceComponent = test
    }
    

    Connections {
        target: ApplicationState.camera
        function onImageCaptured() {
            console.log("captured")
            timer.start()            
        }
    }


}

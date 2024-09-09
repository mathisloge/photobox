import QtQuick
import QtQuick.Controls
import QtQml.StateMachine as DSM
import QtMultimedia
import Photobox.Core

ApplicationWindow {
    visible: true
    color: "black"

    StackView {
        id: stack
        initialItem: liveFeedView
        anchors.fill: parent

        pushEnter: Transition {
            ParallelAnimation {
                NumberAnimation {
                    property: "rotation"
                    from: 270
                    to: 0
                    duration: 1000
                    easing.type: Easing.InCubic
                }
                NumberAnimation {
                    property: "scale"
                    from: 0
                    to: 1
                    duration: 1000
                    easing.type: Easing.InCubic
                }
            }
        }
        pushExit: Transition {
            NumberAnimation {
                property: "scale"
                from: 1
                to: 1
                duration: 1000
                easing.type: Easing.InCubic
            }
        }
        popEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: 200
            }
        }
        popExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: 200
            }
        }
    }

    Component {
        id: liveFeedView

        Item {
            VideoOutput {
                id: videoOutput
                anchors.fill: parent

                Component.onCompleted: {
                    ApplicationState.camera.videoSink = videoOutput.videoSink;
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
                initialCount: 1

                onFinished: {
                    shutter.running = true;
                    ApplicationState.camera.requestCapturePhoto();
                    shutterSound.play();
                }
            }
            Button {
                text: "start"
                visible: !countdown.running
                onClicked: countdown.running = true
            }

            Connections {
                target: ApplicationState.triggerClient
                 function onTriggered() { 
                    if(!countdown.running) {
                        countdown.running = true
                    }
                  }
            }
        }
    }

    Component {
        id: captureView
        Item {
            Image {
                anchors.centerIn: parent
                source: "image://camera/capture"
                cache: false
                fillMode: Image.PreserveAspectFit

                BorderImage {
                    anchors {
                        fill: parent
                        margins: 1
                    }
                    border {
                        left: 30
                        top: 30
                        right: 30
                        bottom: 30
                    }
                    horizontalTileMode: BorderImage.Stretch
                    verticalTileMode: BorderImage.Stretch
                    source: "border.svg"
                }
            }
        }
    }

    DSM.StateMachine {
        id: captureFlow
        initialState: stateLiveFeed
        running: true

        DSM.State {
            id: stateLiveFeed
            DSM.SignalTransition {
                signal: ApplicationState.camera.imageCaptured
                targetState: stateShowCaptureImage
            }
        }

        DSM.State {
            id: stateShowCaptureImage
            DSM.TimeoutTransition {
                targetState: stateFinal
                timeout: 5000
            }
            onEntered: stack.push(captureView)
        }

        DSM.FinalState {
            id: stateFinal
        }

        onFinished: {
            stack.pop(null);
            captureFlow.start();
        }
    }
}

import QtQuick
import QtQuick.Controls
import QtQml.StateMachine as DSM
import QtMultimedia
import Photobox.Core
import Photobox.Ui

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

    Component.onCompleted: {
        ApplicationState.triggerClient.playEffect(PhotoTriggerClient.Idle);
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
                    shutter.running = true;
                    ApplicationState.camera.requestCapturePhoto();
                    shutterSound.play();
                }

                onRunningChanged: {
                    if(this.running) {
                        ApplicationState.triggerClient.playEffect(PhotoTriggerClient.Countdown);
                    } else {
                        ApplicationState.triggerClient.playEffect(PhotoTriggerClient.Idle);
                    }
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
        PreviewImage {
            source: "image://camera/capture"
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

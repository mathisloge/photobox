import Photobox.Core
import Photobox.Ui
import QtMultimedia
import QtQml.StateMachine as DSM
import QtQuick
import QtQuick.Controls

ApplicationWindow {
    visible: true
    color: "black"
    Component.onCompleted: {
        ApplicationState.triggerClient.playEffect(PhotoTriggerClient.Idle);
    }

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

            Countdown {
                id: countdown

                anchors.fill: parent
                visible: running
                initialCount: 5
                onFinished: {
                    ApplicationState.camera.requestCapturePhoto();
                }
                onRunningChanged: {
                    if (this.running)
                        ApplicationState.triggerClient.playEffect(PhotoTriggerClient.Countdown);
                    else
                        ApplicationState.triggerClient.playEffect(PhotoTriggerClient.Idle);
                }
            }

            Button {
                text: "start"
                visible: !countdown.running
                onClicked: countdown.running = true
            }

            Connections {
                function onTriggered() {
                    if (!countdown.running)
                        countdown.running = true;

                }

                target: ApplicationState.triggerClient
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
        onFinished: {
            stack.pop(null);
            captureFlow.start();
        }

        DSM.State {
            id: stateLiveFeed

            DSM.SignalTransition {
                signal: ApplicationState.camera.imageCaptured
                targetState: stateShowCaptureImage
            }

        }

        DSM.State {
            id: stateShowCaptureImage

            onEntered: stack.push(captureView)

            DSM.TimeoutTransition {
                targetState: stateFinal
                timeout: 5000
            }

        }

        DSM.FinalState {
            id: stateFinal
        }

    }

}

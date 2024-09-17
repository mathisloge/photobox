import Photobox.Core
import Photobox.Ui
import QtMultimedia
import QtQml.StateMachine as DSM
import QtQuick
import QtQuick.Controls

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

                Behavior on opacity {
                    NumberAnimation {
                        duration: 500
                    }

                }

            }

            Countdown {
                id: countdown

                anchors.fill: parent
                visible: running
                initialCount: 5
            }

            MouseArea {
                id: startButton

                anchors.fill: parent
            }

            DSM.StateMachine {
                initialState: statePreview
                running: true

                DSM.State {
                    id: statePreview

                    onEntered: {
                        videoOutput.opacity = 1;
                        ApplicationState.triggerClient.playEffect(PhotoTriggerClient.Idle);
                    }

                    DSM.SignalTransition {
                        signal: ApplicationState.triggerClient.triggered
                        targetState: stateBeginCountdown
                    }

                    DSM.SignalTransition {
                        signal: startButton.clicked
                        targetState: stateBeginCountdown
                    }

                }

                DSM.State {
                    id: stateBeginCountdown

                    onEntered: {
                        countdown.running = true;
                        ApplicationState.triggerClient.playEffect(PhotoTriggerClient.Countdown);
                    }

                    DSM.SignalTransition {
                        signal: countdown.countChanged
                        targetState: stateBeforeCapture
                        guard: countdown.count <= 1
                    }

                }

                DSM.State {
                    id: stateBeforeCapture

                    onEntered: {
                        videoOutput.opacity = 0;
                    }

                    DSM.SignalTransition {
                        signal: countdown.finished
                        targetState: stateCapturePhoto
                    }

                }

                DSM.State {
                    id: stateCapturePhoto

                    onEntered: {
                        ApplicationState.camera.requestCapturePhoto();
                    }

                    DSM.SignalTransition {
                        signal: ApplicationState.camera.imageCaptured
                        targetState: statePreview
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
        id: applicationFlow

        initialState: stateLiveFeed
        running: true
        onFinished: {
            stack.pop(null);
            applicationFlow.start();
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

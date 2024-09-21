import Photobox.Core
import Photobox.Ui
import QtMultimedia
import QtQml.StateMachine as DSM
import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: window

    property var currentImage: ""

    signal capturedPreviewFinished()

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

            Flow {
                id: previewImages

                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                spacing: 10
                flow: Flow.TopToBottom

                Repeater {
                    model: ApplicationState.captureController.model

                    delegate: PreviewImage {
                        width: 200
                        fillMode: Image.PreserveAspectFit
                        source: "image://camera/" + model.source
                    }

                }

                add: Transition {
                    id: previewAddTransition

                    SequentialAnimation {
                        PropertyAction {
                            property: "x"
                            value: -previewAddTransition.ViewTransition.item.width
                        }

                        ScriptAction {
                            script: {
                                window.currentImage = previewAddTransition.ViewTransition.item.source;
                                stack.push(captureView);
                            }
                        }

                        PauseAnimation {
                            duration: 5000
                        }

                        ScriptAction {
                            script: stack.pop(null)
                        }

                        PauseAnimation {
                            duration: 500
                        }

                        ScriptAction {
                            script: window.capturedPreviewFinished()
                        }

                        NumberAnimation {
                            properties: "x"
                            duration: 1000
                            easing.type: Easing.InOutCubic
                        }

                    }

                }

            }

            Countdown {
                id: countdown

                anchors.fill: parent
                visible: running
                initialCount: 1
            }

            MouseArea {
                id: startButton

                anchors.fill: parent
            }

            DSM.StateMachine {
                id: captureStateMachine

                initialState: idleState
                running: true

                DSM.State {
                    id: idleState

                    onEntered: {
                        videoOutput.opacity = 1;
                        ApplicationState.triggerClient.playEffect(PhotoTriggerClient.Idle);
                    }

                    DSM.SignalTransition {
                        signal: ApplicationState.triggerClient.triggered
                        targetState: collageCapture
                    }

                    DSM.SignalTransition {
                        signal: startButton.clicked
                        targetState: collageCapture
                    }

                }

                DSM.State {
                    id: collageCapture

                    initialState: stateBeginCountdown
                    onExited: {
                        ApplicationState.captureController.reset();
                    }

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
                            ApplicationState.captureController.captureImage();
                        }

                        DSM.SignalTransition {
                            signal: ApplicationState.captureController.collageCaptureComplete
                            targetState: stateShowCollageFinal
                        }

                        DSM.SignalTransition {
                            signal: window.capturedPreviewFinished
                            targetState: statePreview
                        }

                    }

                    DSM.State {
                        // todo: push collage image onto stack after everything was stored.

                        id: stateShowCollageFinal

                        DSM.TimeoutTransition {
                            targetState: idleState
                            timeout: 5000
                        }

                    }

                }

            }

        }

    }

    Component {
        id: captureView

        PreviewImage {
            source: window.currentImage
        }

    }

}

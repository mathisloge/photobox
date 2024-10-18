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
            PropertyAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: 500
            }

        }

        pushExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: 500
            }

        }

        popEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: 500
            }

        }

        popExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: 500
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
                initialCount: 10
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
                        stack.pop(null);
                        videoOutput.opacity = 1;
                        ApplicationState.remoteTrigger.playEffect(RemoteTrigger.Idle);
                    }

                    DSM.SignalTransition {
                        signal: ApplicationState.remoteTrigger.triggered
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
                            stack.pop(null);
                            videoOutput.opacity = 1;
                            ApplicationState.remoteTrigger.playEffect(RemoteTrigger.Idle);
                        }

                        DSM.SignalTransition {
                            signal: ApplicationState.remoteTrigger.triggered
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
                            ApplicationState.remoteTrigger.playEffect(RemoteTrigger.Countdown);
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
                            stack.push(busyComponent);
                        }

                        DSM.SignalTransition {
                            signal: ApplicationState.captureController.capturedImageReady
                            targetState: stateShowCapturedImage
                        }

                    }

                    DSM.State {
                        id: stateShowCapturedImage

                        onEntered: {
                            stack.push(captureView);
                        }

                        DSM.TimeoutTransition {
                            targetState: ApplicationState.captureController.collageComplete ? stateBusyCaptureWait : statePreview
                            timeout: 5000
                        }

                        DSM.SignalTransition {
                            signal: ApplicationState.captureController.collageCaptureComplete
                            guard: ApplicationState.captureController.collageComplete
                            targetState: stateShowCollageFinal
                        }

                    }

                    DSM.State {
                        id: stateBusyCaptureWait

                        onEntered: {
                            stack.push(busyComponent);
                        }

                        DSM.SignalTransition {
                            signal: ApplicationState.captureController.collageCaptureComplete
                            targetState: stateShowCollageFinal
                        }

                    }

                    DSM.State {
                        id: stateShowCollageFinal

                        onEntered: {
                            stack.push(collageView);
                        }

                        DSM.TimeoutTransition {
                            targetState: idleState
                            timeout: 10000
                        }

                    }

                }

            }

        }

    }

    Component {
        id: captureView

        PreviewImage {
            source: "image://camera/" + ApplicationState.captureController.model.sourceOfLastItem()
        }

    }

    Component {
        id: collageView

        PreviewImage {
            source: "file://" + ApplicationState.captureController.collageImagePath
        }

    }

    Component {
        id: busyComponent

        BusyPage {
        }

    }

}

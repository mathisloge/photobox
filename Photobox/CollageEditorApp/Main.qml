import Photobox.Core
import Photobox.Ui
import QtMultimedia
import QtQml.StateMachine as DSM
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    property int resolutionScale: 50
    property int layoutHeight: 10 * resolutionScale
    property int layoutWidth: 15 * resolutionScale

    visible: true
    color: "black"
    width: 720
    height: 480

    RowLayout {
        id: layout

        anchors.fill: parent
        spacing: 10

        Rectangle {
            color: 'white'
            width: layoutWidth
            height: layoutHeight

            DropArea {
                anchors.fill: parent

                Rectangle {
                    anchors.fill: parent
                    color: "green"
                    visible: parent.containsDrag
                }

            }

        }

        Rectangle {
            color: 'white'
            Layout.fillHeight: true
            Layout.minimumWidth: 150
            Layout.preferredWidth: 200

            ColumnLayout {
                width: parent.width

                // https://www.adobe.com/de/creativecloud/photography/discover/standard-photo-sizes.html
                ComboBox {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop
                    editable: false
                    onAccepted: {
                    }

                    model: ListModel {
                        id: model

                        ListElement {
                            text: "10 x 15"
                        }

                        ListElement {
                            text: "13 x 18"
                        }

                        ListElement {
                            text: "A4"
                        }

                    }

                }

                Rectangle {
                    id: container

                    color: "orange"
                    Layout.preferredHeight: 50
                    Layout.preferredWidth: 50
                    Layout.alignment: Qt.AlignTop

                    Component {
                        id: itemComponent

                        Rectangle {
                            width: 50
                            height: 50
                            //Layout.fillWidth: true
                            color: "red"
                            Drag.hotSpot.x: 10
                            Drag.hotSpot.y: 10
                            Drag.dragType: Drag.Automatic
                            Drag.supportedActions: Qt.CopyAction

                            DragHandler {
                            }

                            WheelHandler {
                                property: "rotation"
                            }

                        }

                    }

                    MouseArea {
                        id: mouseArea

                        function addItem(x, y) {
                            var obj = itemComponent.createObject(container, {
                                "x": x - 25,
                                "y": y - 25
                            });
                            mouseArea.drag.target = obj;
                            Drag.active = true;
                        }

                        anchors.fill: parent
                        onPressed: (mouse) => {
                            addItem(mouse.x, mouse.y);
                        }
                        onReleased: {
                            Drag.active = false;
                        }
                    }

                }

            }

        }

    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")

            Action {
                text: qsTr("&New...")
            }

            Action {
                text: qsTr("&Open...")
            }

            Action {
                text: qsTr("&Save")
            }

            Action {
                text: qsTr("Save &As...")
            }

            MenuSeparator {
            }

            Action {
                text: qsTr("&Quit")
            }

        }

    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent

            ToolButton {
                text: qsTr("Import graphic")
                onClicked: {
                }
            }

            ToolButton {
                text: qsTr("Print to pdf")
                onClicked: {
                }
            }

            ToolButton {
                text: qsTr("Capture reference image")
                onClicked: {
                }
            }

            Item {
                Layout.fillWidth: true
            }

        }

    }

}

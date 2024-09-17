import Photobox.Core
import Photobox.Ui
import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    color: "black"
    width: 720
    height: 480

    ListModel {
        id: imageElements
    }

    Popup {
        id: popup

        anchors.centerIn: Overlay.overlay
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

        contentItem: ColumnLayout {
            TextField {
                id: textInput

                width: 250
            }

            RowLayout {
                Button {
                    text: "Hinzufügen"
                    onClicked: imageElements.append({
                        "elementId": textInput.text
                    })
                    enabled: textInput.text
                }

                Button {
                    text: "Abbrechen"
                    onClicked: popup.close()
                }

            }

        }

    }

    RowLayout {
        anchors.fill: parent

        Pane {
            Layout.minimumWidth: 200
            Layout.preferredWidth: 200
            Layout.fillHeight: true

            ListView {
                anchors.fill: parent
                spacing: 3
                model: imageElements

                delegate: Frame {
                    //color: "salmon"
                    width: ListView.view.width
                    height: 50

                    DelayButton {
                        anchors.top: parent.top
                        anchors.right: parent.right
                        onActivated: imageElements.remove(index)
                        icon.name: "edit-delete"
                    }

                    Label {
                        text: model.elementId
                    }

                }

            }

        }

        CollageImage {
            ///fillMode: Image.PreserveAspectFit

            id: image

            Layout.fillWidth: true
            Layout.fillHeight: true
        }

    }

    FileDialog {
        id: fileDialog

        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        onAccepted: image.loadSource(selectedFile)
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent

            ToolButton {
                text: "Add image element"
                onClicked: {
                    popup.open();
                }
            }

            ToolButton {
                text: "Load template"
                onClicked: fileDialog.open()
            }

            ToolButton {
                text: "Printer settings"
                onClicked: {
                }
            }

        }

    }

}

// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

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

    PrintController {
        id: printController
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
                    onClicked: {
                        image.model.appendElement(textInput.text);
                    }
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
                model: image.model

                delegate: Frame {
                    width: ListView.view.width
                    height: 50

                    DelayButton {
                        anchors.top: parent.top
                        anchors.right: parent.right
                        onActivated: {
                            image.model.removeElement(model.elementId);
                        }
                        icon.name: "edit-delete"
                    }

                    Label {
                        text: model.elementId
                    }

                    Button {
                        text: "Testfoto"
                        onClicked: {
                            fileSetImageDialog.currentElement = model.elementId;
                            fileSetImageDialog.open();
                        }
                    }
                }
            }
        }

        CollageImage {
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

    FileDialog {
        id: fileSetImageDialog

        property string currentElement: ""

        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        onAccepted: {
            image.setSourceOfPhoto(fileSetImageDialog.currentElement, selectedFile);
        }
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent

            ToolButton {
                text: "Load template"
                onClicked: fileDialog.open()
            }

            ToolButton {
                text: "Add image element"
                onClicked: {
                    popup.open();
                }
            }

            ToolButton {
                text: "Printer settings"
                onClicked: {
                    printController.showPrintSettings();
                }
            }

            ToolButton {
                text: "Save collage"
                onClicked: {
                    image.saveConfiguration();
                }
            }
        }
    }
}

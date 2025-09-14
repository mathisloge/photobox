// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Photobox.Ui

Page {
    ListView {
        anchors.fill: parent
        spacing: 10

        model: ListModel {
            ListElement {
                name: "RemoteTriggerSingle"
                url: "http://192.168.178.30"
            }
            ListElement {
                name: "RemoteTriggerCollage"
                url: "http://192.168.178.30"
            }
        }
        delegate: Frame {
            id: delegate
            required property string name
            required property string url

            width: parent.width

            RowLayout {
                id: layout
                width: parent.width
                ColumnLayout {
                    Layout.fillWidth: true

                    SystemStatusDelegate {
                        fontSize: 24
                        title: delegate.name
                        status: 0
                    }

                    Label {
                        text: delegate.url
                    }
                }
                Button {
                    Layout.alignment: Qt.AlignRight
                    text: "Edit"
                    onClicked: {}
                }
            }
        }
    }
}

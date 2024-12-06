// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Photobox.Ui

Page {
    ListView {
        width: 180
        height: 200

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
            RowLayout {
                ColumnLayout {
                    SystemStatusDelegate {
                        fontSize: 24
                        title: delegate.name
                        status: 1
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

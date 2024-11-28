// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Photobox.Core

RowLayout {
    id: root
    required property string title
    required property int status

    readonly property color errorStatus: "#E51400"
    readonly property color errorStatusBorder: "#A20025"
    readonly property color okStatus: "#60A917"
    readonly property color okStatusBorder: "#008A00"

    property color __statusColor: errorStatus
    property color __statusColorBorder: errorStatusBorder

    Loader {
        id: loader
        Layout.preferredHeight: label.height
        Layout.preferredWidth: label.height
        Layout.alignment: Qt.AlignHCenter
        sourceComponent: ledStatusComponent
    }

    Label {
        id: label
        text: root.title
        font.pixelSize: 42
    }

    Component {
        id: ledStatusComponent
        Rectangle {
            radius: 180
            color: root.__statusColor
            border.color: root.__statusColorBorder
            border.width: 5
        }
    }

    Component {
        id: busyComponent

        BusyIndicator {
            running: true
        }
    }

    states: [
        State {
            when: root.status === SystemStatusCode.Connecting
            PropertyChanges {
                loader.sourceComponent: busyComponent
            }
        },
        State {
            when: root.status === SystemStatusCode.Error
            PropertyChanges {
                root.__statusColor: root.errorStatus
                root.__statusColorBorder: root.errorStatusBorder
            }
        },
        State {
            when: root.status === SystemStatusCode.Ok
            PropertyChanges {
                root.__statusColor: root.okStatus
                root.__statusColorBorder: root.okStatusBorder
            }
        }
    ]
}

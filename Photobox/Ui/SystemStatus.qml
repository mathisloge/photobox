pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    id: root

    enum Status {
        Connecting,
        Error,
        Ok
    }

    property alias title: label.text
    property int status: SystemStatus.Status.Error

    readonly property color errorStatus: "#E51400"
    readonly property color errorStatusBorder: "#A20025"
    readonly property color okStatus: "#60A917"
    readonly property color okStatusBorder: "#008A00"

    property color statusColor: errorStatus
    property color statusColorBorder: errorStatusBorder

    Loader {
        id: loader
        Layout.preferredHeight: label.height
        Layout.preferredWidth: label.height
        Layout.alignment: Qt.AlignHCenter
        sourceComponent: ledStatusComponent
    }

    Label {
        id: label
    }

    Component {
        id: ledStatusComponent
        Rectangle {
            radius: 180
            color: root.statusColor
            border.color: root.statusColorBorder
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
            when: root.status === SystemStatus.Status.Connecting
            PropertyChanges {
                loader.sourceComponent: busyComponent
            }
        },
        State {
            when: root.status === SystemStatus.Status.Error
            PropertyChanges {
                root.statusColor: root.errorStatus
                root.statusColorBorder: root.errorStatusBorder
            }
        },
        State {
            when: root.status === SystemStatus.Status.Ok
            PropertyChanges {
                root.statusColor: root.okStatus
                root.statusColorBorder: root.okStatusBorder
            }
        }
    ]
}

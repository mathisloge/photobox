import QtQuick
import QtQuick.Controls
import Photobox.Core

Page {
    id: root
    readonly property ICaptureSession session: ApplicationState.captureManager.session

    StackView {
        id: stack
        initialItem: capturePage
        anchors.fill: parent
    }

    states: [
        State {
            name: "capturing"
            when: root.session.status !== ICaptureSession.Busy
            StateChangeScript {
                script: stack.pop(null)
            }
        },
        State {
            name: "busy"
            when: root.session.status === ICaptureSession.Busy
            StateChangeScript {
                script: stack.push(busyPage)
            }
        }
    ]

    Component {
        id: capturePage
        CapturePage {}
    }

    Component {
        id: busyPage
        BusyPage {}
    }
}

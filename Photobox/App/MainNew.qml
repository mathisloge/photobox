import Photobox.Core
import Photobox.Ui
import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: window

    StackView {
        id: stackView

        anchors.fill: parent
        initialItem: statusPage

        states: [
            State {
                name: "status"
                when: ApplicationState.systemStatusManager.status === SystemStatusManager.Initializing
                StateChangeScript {
                    script: stackView.pop(null)
                }
            },
            State {
                name: "normal"
                when: ApplicationState.systemStatusManager.status === SystemStatusManager.RequiredClientsInitilized || ApplicationState.systemStatusManager.status === SystemStatusManager.AllClientsInitializied
                StateChangeScript {
                    script: stackView.push(capturePage)
                }
            }
        ]
    }

    Component {
        id: statusPage

        StatusPage {}
    }

    Component {
        id: capturePage

        CapturePage {}
    }
}

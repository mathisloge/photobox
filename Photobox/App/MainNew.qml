import Photobox.Ui
import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: window

    StackView {
        id: stackView

        anchors.fill: parent
        initialItem: statusPage
    }

    //! TODO: switch from status page, if the required system components are ok
    //! switch to status page, if any of the required system components are not ok
    Timer {
        interval: 1000
        running: true
        repeat: false
        onTriggered: stackView.push(capturePage)
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

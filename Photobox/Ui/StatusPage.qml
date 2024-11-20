import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Photobox.Core

Page {
    ColumnLayout {
        anchors.fill: parent

        SystemStatusDelegate {
            title: "Camera"
            status: SystemStatusCode.Connecting
        }

        SystemStatusDelegate {
            title: "Buzzer"
            status: SystemStatusCode.Ok
        }

        SystemStatusDelegate {
            title: "Camera"
            status: SystemStatusCode.Error
        }
    }
}

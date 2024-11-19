import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    ColumnLayout {
        anchors.fill: parent

        SystemStatus {
            title: "Camera"
            status: SystemStatus.Status.Connecting
        }

        SystemStatus {
            title: "Buzzer"
            status: SystemStatus.Status.Ok
        }

        SystemStatus {
            title: "Camera"
            status: SystemStatus.Status.Error
        }
    }
}

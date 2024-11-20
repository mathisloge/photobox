import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Photobox.Core

Page {
    ColumnLayout {
        anchors.fill: parent
        Image {
            Layout.alignment: Qt.AlignHCenter
            source: "Logo.svg"
        }
        ListView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            delegate: SystemStatusDelegate {}
            model: ApplicationState.systemStatusManager.model
        }
    }
}

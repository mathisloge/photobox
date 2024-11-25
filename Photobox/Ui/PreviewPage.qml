import QtQuick
import QtQuick.Controls

Page {
    property alias imageSource: previewImage.source
    PreviewImage {
        id: previewImage
        anchors.fill: parent
    }
}

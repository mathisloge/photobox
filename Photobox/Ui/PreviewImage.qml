import QtQuick

Image {
    cache: false
    fillMode: Image.PreserveAspectFit

    BorderImage {
        horizontalTileMode: BorderImage.Stretch
        verticalTileMode: BorderImage.Stretch
        source: "ImageBorder.svg"

        anchors {
            fill: parent
            margins: 1
        }

        border {
            left: 30
            top: 30
            right: 30
            bottom: 30
        }

    }

}

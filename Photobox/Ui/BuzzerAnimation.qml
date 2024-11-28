// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Layouts

Item {
    id: root
    implicitWidth: buzzerImage.implicitWidth
    implicitHeight: pointerImage.implicitHeight + buzzerImage.implicitHeight
    Image {
        id: pointerImage
        source: "Pointer.svg"
        Layout.alignment: Qt.AlignHCenter
        SequentialAnimation on anchors.bottomMargin {
            loops: Animation.Infinite
            PropertyAnimation {
                to: 100
                duration: 2000
                easing.type: Easing.InOutQuad
            }
            PropertyAnimation {
                to: 0
                duration: 2000
                easing.type: Easing.InOutQuad
            }
        }
        anchors.horizontalCenter: root.horizontalCenter
        anchors.bottom: buzzerImage.top
    }
    Image {
        id: buzzerImage
        anchors.horizontalCenter: root.horizontalCenter
        anchors.bottom: root.bottom
        source: "Buzzer.svg"
    }

    Behavior on opacity {
        NumberAnimation {
            duration: 500
        }
    }
}

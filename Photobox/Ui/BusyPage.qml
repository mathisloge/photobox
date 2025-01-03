// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick.Controls
import QtQuick.Layouts

Page {
    ColumnLayout {
        anchors.centerIn: parent

        BusyIndicator {
            Layout.alignment: Qt.AlignHCenter
            running: true
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: "Bitte warten"
            font.pointSize: 40
        }

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: "Die Bilder werden verarbeitet"
            font.pointSize: 30
        }
    }
}

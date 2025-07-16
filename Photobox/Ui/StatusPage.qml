// SPDX-FileCopyrightText: 2024 Mathis Logemann <mathisloge.opensource@pm.me>
// SPDX-FileCopyrightText: 2025 Mathis Logemann <mathisloge.opensource@pm.me>
//
// SPDX-License-Identifier: GPL-3.0-or-later

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

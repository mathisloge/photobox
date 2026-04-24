// SPDX-FileCopyrightText: 2024 - 2025 Mathis Logemann <mathis.opensource@tuta.io>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls

Page {
    property alias imageSource: previewImage.source
    background: PageBackground {}

    PreviewImage {
        id: previewImage
        anchors.centerIn: parent
        fillMode: Image.PreserveAspectCrop
        width: parent.width / 2
        height: parent.height / 2
    }
}

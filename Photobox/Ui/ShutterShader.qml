// Created with Qt Quick Effect Maker (version 0.43), Sun Sep 8 11:06:36 2024

import QtQuick

Item {
    id: rootItem

    // This is the main source for the effect
    property Item source: null
    // Enable this to animate iTime property
    property alias timeRunning: frameAnimation.running
    // When timeRunning is false, this can be used to control iTime manually
    property real animatedTime: frameAnimation.elapsedTime

    FrameAnimation {
        id: frameAnimation

        running: rootItem.timeRunning
        onTriggered: {
            if (elapsedTime > 1.3)
                stop();

        }
    }

    ShaderEffect {
        readonly property alias iSource: rootItem.source
        readonly property alias iTime: rootItem.animatedTime
        readonly property vector3d iResolution: Qt.vector3d(width, height, 1)

        vertexShader: 'ShutterShader/shuttershader.vert.qsb'
        fragmentShader: 'ShutterShader/shuttershader.frag.qsb'
        anchors.fill: parent
    }

}

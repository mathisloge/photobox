import QtQuick
import QtQuick.Controls
import QtQuick.Shapes

Item {
    id: root
    property alias running: animation.running

    readonly property var __upperTriangle: [Qt.point(100.0, 0.0), Qt.point(-50, 100.0), Qt.point(-50, -100.0)]
    readonly property var __lowerTriangle: [Qt.point(50.0, 100.0), Qt.point(-100.0, 0.0), Qt.point(50.0, -100.0)]
    readonly property int __animTime: 200
    Shape {
        anchors.centerIn: parent
        transform: Translate {
            x: 25
        }

        ShapePath {
            id: triangle0
            strokeColor: "black"
            fillColor: "black"
            fillRule: ShapePath.WindingFill

            startX: 0
            startY: 0
            PathLine {
                relativeX: __upperTriangle[0].x
                relativeY: __upperTriangle[0].y
            }
            PathLine {
                relativeX: __upperTriangle[1].x
                relativeY: __upperTriangle[1].y
            }
            PathLine {
                relativeX: __upperTriangle[2].x
                relativeY: __upperTriangle[2].y
            }
        }

        ShapePath {
            id: triangle1
            strokeColor: "black"
            fillColor: "black"
            fillRule: ShapePath.WindingFill

            startX: 100
            startY: 0
            PathLine {
                relativeX: __lowerTriangle[0].x
                relativeY: __lowerTriangle[0].y
            }
            PathLine {
                relativeX: __lowerTriangle[1].x
                relativeY: __lowerTriangle[1].y
            }
            PathLine {
                relativeX: __lowerTriangle[2].x
                relativeY: __lowerTriangle[2].y
            }
        }

        ShapePath {
            id: triangle2
            strokeColor: "black"
            fillColor: "black"
            fillRule: ShapePath.WindingFill

            startX: 50
            startY: 100
            PathLine {
                relativeX: __upperTriangle[0].x
                relativeY: __upperTriangle[0].y
            }
            PathLine {
                relativeX: __upperTriangle[1].x
                relativeY: __upperTriangle[1].y
            }
            PathLine {
                relativeX: __upperTriangle[2].x
                relativeY: __upperTriangle[2].y
            }
        }

        ShapePath {
            id: triangle3
            strokeColor: "black"
            fillColor: "black"
            fillRule: ShapePath.WindingFill

            startX: 50
            startY: 100
            PathLine {
                relativeX: __lowerTriangle[0].x
                relativeY: __lowerTriangle[0].y
            }
            PathLine {
                relativeX: __lowerTriangle[1].x
                relativeY: __lowerTriangle[1].y
            }
            PathLine {
                relativeX: __lowerTriangle[2].x
                relativeY: __lowerTriangle[2].y
            }
        }

        ShapePath {
            id: triangle4
            strokeColor: "black"
            fillColor: "black"
            fillRule: ShapePath.WindingFill

            startX: -50
            startY: 100
            PathLine {
                relativeX: __upperTriangle[0].x
                relativeY: __upperTriangle[0].y
            }
            PathLine {
                relativeX: __upperTriangle[1].x
                relativeY: __upperTriangle[1].y
            }
            PathLine {
                relativeX: __upperTriangle[2].x
                relativeY: __upperTriangle[2].y
            }
        }

        ShapePath {
            id: triangle5
            strokeColor: "black"
            fillColor: "black"
            fillRule: ShapePath.WindingFill

            startX: 0
            startY: 0
            PathLine {
                relativeX: __lowerTriangle[0].x
                relativeY: __lowerTriangle[0].y
            }
            PathLine {
                relativeX: __lowerTriangle[1].x
                relativeY: __lowerTriangle[1].y
            }
            PathLine {
                relativeX: __lowerTriangle[2].x
                relativeY: __lowerTriangle[2].y
            }
        }
    }

    SequentialAnimation {
        id: animation
        running: true

        ParallelAnimation {
            PropertyAnimation {
                target: triangle0
                property: "startX"
                from: 100
                to: 0
                duration: __animTime
            }
            PropertyAnimation {
                target: triangle0
                property: "startY"
                from: 0
                to: 0
                duration: __animTime
            }

            PropertyAnimation {
                target: triangle1
                property: "startX"
                from: 150
                to: 100
                duration: __animTime
            }
            PropertyAnimation {
                target: triangle1
                property: "startY"
                from: 100
                to: 0
                duration: __animTime
            }

            PropertyAnimation {
                target: triangle2
                property: "startX"
                from: 0
                to: 50
                duration: __animTime
            }
            PropertyAnimation {
                target: triangle2
                property: "startY"
                from: 200
                to: 100
                duration: __animTime
            }

            PropertyAnimation {
                target: triangle3
                property: "startX"
                from: -50
                to: 50
                duration: __animTime
            }

            PropertyAnimation {
                target: triangle4
                property: "startX"
                from: -100
                to: -50
                duration: __animTime
            }
            PropertyAnimation {
                target: triangle4
                property: "startY"
                from: 0
                to: 100
                duration: __animTime
            }

            PropertyAnimation {
                target: triangle5
                property: "startX"
                from: 50
                to: 0
                duration: __animTime
            }
            PropertyAnimation {
                target: triangle5
                property: "startY"
                from: -100
                to: 0
                duration: __animTime
            }
        }

        PauseAnimation {
            duration: 100
        }

        ParallelAnimation {
            PropertyAnimation {
                target: triangle0
                property: "startX"
                from: 0
                to: 100
                duration: __animTime
            }
            PropertyAnimation {
                target: triangle0
                property: "startY"
                from: 0
                to: 0
                duration: __animTime
            }

            PropertyAnimation {
                target: triangle1
                property: "startX"
                from: 100
                to: 150
                duration: __animTime
            }
            PropertyAnimation {
                target: triangle1
                property: "startY"
                from: 0
                to: 100
                duration: __animTime
            }

            PropertyAnimation {
                target: triangle2
                property: "startX"
                from: 50
                to: 0
                duration: __animTime
            }
            PropertyAnimation {
                target: triangle2
                property: "startY"
                from: 100
                to: 200
                duration: __animTime
            }

            PropertyAnimation {
                target: triangle3
                property: "startX"
                from: 50
                to: -50
                duration: __animTime
            }

            PropertyAnimation {
                target: triangle4
                property: "startX"
                from: -50
                to: -100
                duration: __animTime
            }
            PropertyAnimation {
                target: triangle4
                property: "startY"
                from: 100
                to: 0
                duration: __animTime
            }

            PropertyAnimation {
                target: triangle5
                property: "startX"
                from: 0
                to: 50
                duration: __animTime
            }
            PropertyAnimation {
                target: triangle5
                property: "startY"
                from: 0
                to: -100
                duration: __animTime
            }
        }
    }
}

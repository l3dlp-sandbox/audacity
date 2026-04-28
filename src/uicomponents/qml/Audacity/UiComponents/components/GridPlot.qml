import QtQuick 2.15
import Muse.Ui
import Muse.UiComponents

Item {
    id: root

    default property alias content: contentSlot.data

    required property var xTicks
    required property var yTicks

    enum TickPosition {
        Top,
        Bottom,
        Left,
        Right
    }

    property int xTickPosition: GridPlot.Top
    property int yTickPosition: GridPlot.Right

    QtObject {
        id: prv

        readonly property int tickLength: 4
        readonly property int labelWidth: fontMetrics.boundingRect("-000").width
        readonly property int labelHeight: fontMetrics.boundingRect("0").height
        readonly property int labelMargin: 4
        readonly property int extraLabelSpace: 8
    }

    FontMetrics {
        id: fontMetrics
        font.family: ui.theme.bodyFont.family
        font.pixelSize: ui.theme.bodyFont.pixelSize
    }

    Rectangle {
        id: background

        anchors.fill: parent
        anchors.topMargin: root.xTickPosition === GridPlot.Top ? prv.labelHeight + prv.labelMargin : 0
        anchors.bottomMargin: root.xTickPosition === GridPlot.Bottom ? prv.labelHeight + prv.labelMargin : 0
        anchors.leftMargin: root.yTickPosition === GridPlot.Left ? prv.labelWidth + prv.labelMargin + prv.extraLabelSpace : 0
        anchors.rightMargin: root.yTickPosition === GridPlot.Right ? prv.labelWidth + prv.labelMargin + prv.extraLabelSpace : 0

        color: ui.theme.extra["dynamics_background_color"]

        Repeater {
            id: verticalLines

            model: root.xTicks
            delegate: Item {
                x: background.width * index / (root.xTicks.length - 1)
                y: root.xTickPosition === GridPlot.Top ? -prv.tickLength : 0

                StyledTextLabel {
                    width: prv.labelWidth
                    height: prv.labelHeight
                    horizontalAlignment: Text.AlignHCenter

                    anchors.horizontalCenter: vLine.horizontalCenter
                    anchors.bottom: root.xTickPosition === GridPlot.Top ? vLine.top : undefined
                    anchors.top: root.xTickPosition === GridPlot.Bottom ? vLine.bottom : undefined
                    anchors.bottomMargin: root.xTickPosition === GridPlot.Top ? prv.labelMargin : undefined
                    anchors.topMargin: root.xTickPosition === GridPlot.Bottom ? prv.labelMargin : undefined

                    text: modelData
                }

                Rectangle {
                    id: vLine

                    width: 1
                    height: background.height + prv.tickLength
                    color: ui.theme.extra["dynamics_grid_color"]
                }
            }
        }

        Repeater {
            id: horizontalLines
            model: root.yTicks
            delegate: Item {
                x: root.yTickPosition === GridPlot.Right ? 0 : -prv.tickLength
                y: background.height * (1 - index / (root.yTicks.length - 1))

                StyledTextLabel {
                    width: prv.labelWidth
                    height: prv.labelHeight
                    horizontalAlignment: Text.AlignRight
                    anchors.left: root.xTickPosition === GridPlot.Top ? hLine.right : undefined
                    anchors.right: root.xTickPosition === GridPlot.Bottom ? hLine.left : undefined
                    anchors.leftMargin: root.xTickPosition === GridPlot.Top ? prv.labelMargin : undefined
                    anchors.rightMargin: root.xTickPosition === GridPlot.Bottom ? prv.labelMargin : undefined
                    y: hLine.y - (fontMetrics.ascent + fontMetrics.descent) / 2
                    text: modelData
                }

                Rectangle {
                    id: hLine

                    width: background.width + prv.tickLength
                    height: 1
                    color: ui.theme.extra["dynamics_grid_color"]
                }
            }
        }

        Item {
            id: contentSlot
            anchors.fill: parent
        }
    }
}

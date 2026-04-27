import QtQuick 2.15
import Muse.Ui
import Muse.UiComponents

Item {
    id: root

    // dB axis
    property real dbMin
    property real dbMax
    property int dbStep: 6

    // frequency axis (log)
    property real loFreq
    property real hiFreq
    property var freqTicks: [50, 100, 500, 1000, 5000, 10000, 20000]

    property bool linesVisible: true

    readonly property int leftPadding: prv.dbLabelWidth + prv.labelMargin
    readonly property int bottomPadding: prv.labelHeight + prv.labelMargin

    // Plot area inside which the curve gets drawn — so the PolylinePlot can mirror it.
    readonly property int plotX: leftPadding
    readonly property int plotY: 0
    readonly property int plotW: width - leftPadding
    readonly property int plotH: height - bottomPadding

    QtObject {
        id: prv

        readonly property int labelMargin: 4
        readonly property int dbLabelWidth: fontMetrics.boundingRect("-30dB").width
        readonly property int labelHeight: fontMetrics.boundingRect("0").height

        readonly property real loLog: Math.log(root.loFreq) / Math.LN10
        readonly property real hiLog: Math.log(root.hiFreq) / Math.LN10
        readonly property real denom: prv.hiLog - prv.loLog

        readonly property var dbTicks: (function() {
                const ticks = []
                const start = Math.ceil(root.dbMin / root.dbStep) * root.dbStep
                const end = Math.floor(root.dbMax / root.dbStep) * root.dbStep
                for (let v = start; v <= end; v += root.dbStep) {
                    ticks.push(v)
                }
                return ticks
            })()

        function freqToNorm(f) {
            if (f <= 0 || prv.denom <= 0) {
                return 0
            }
            return (Math.log(f) / Math.LN10 - prv.loLog) / prv.denom
        }

        function freqLabel(f) {
            if (f >= 1000) {
                const k = f / 1000
                return (Math.round(k * 10) / 10).toString() + "k"
            }
            return f.toString()
        }
    }

    FontMetrics {
        id: fontMetrics
        font.family: ui.theme.bodyFont.family
        font.pixelSize: ui.theme.bodyFont.pixelSize
    }

    // dB labels + horizontal gridlines
    Repeater {
        model: prv.dbTicks

        delegate: Item {
            y: root.plotY + root.plotH * (1 - (modelData - root.dbMin) / (root.dbMax - root.dbMin))

            StyledTextLabel {
                width: prv.dbLabelWidth
                horizontalAlignment: Text.AlignRight
                anchors.right: hLine.left
                anchors.rightMargin: prv.labelMargin
                y: -(fontMetrics.ascent + fontMetrics.descent) / 2
                text: modelData + "dB"
            }

            Rectangle {
                id: hLine
                x: root.plotX
                width: root.plotW
                height: 1
                color: ui.theme.extra["graphic_eq_gridline_color"]
                visible: root.linesVisible
            }
        }
    }

    // Frequency labels + vertical gridlines
    Repeater {
        model: root.freqTicks

        delegate: Item {
            readonly property real norm: prv.freqToNorm(modelData)
            x: root.plotX + root.plotW * norm
            visible: norm >= 0 && norm <= 1

            Rectangle {
                id: vLine
                y: root.plotY
                width: 1
                height: root.plotH
                color: ui.theme.extra["graphic_eq_gridline_color"]
                visible: root.linesVisible
            }

            StyledTextLabel {
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: vLine.horizontalCenter
                anchors.top: vLine.bottom
                anchors.topMargin: prv.labelMargin
                text: prv.freqLabel(modelData)
            }
        }
    }
}

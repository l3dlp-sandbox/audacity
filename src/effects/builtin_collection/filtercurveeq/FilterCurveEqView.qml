import QtQuick
import QtQuick.Controls
import Muse.Ui
import Muse.UiComponents
import Audacity.Effects
import Audacity.BuiltinEffects
import Audacity.BuiltinEffectsCollection

BuiltinEffectBase {
    id: root

    property string title: qsTrc("effects/filtercurveeq", "Filter Curve EQ")
    property bool isApplyAllowed: true

    width: boardRectangle.width
    implicitHeight: boardRectangle.height

    builtinEffectModel: FilterCurveEqViewModelFactory.createModel(root, root.instanceId)
    numNavigationPanels: 1
    property alias filterCurveEq: root.builtinEffectModel
    property NavigationPanel buttonsNavigationPanel: NavigationPanel {
        name: "FilterCurveEqButtons"
        enabled: root.enabled && root.visible
        direction: NavigationPanel.Horizontal
        section: root.dialogView ? root.dialogView.navigationSection : null
        order: 1
    }

    Rectangle {
        id: boardRectangle

        width: 720
        height: 480
        anchors.centerIn: parent

        radius: 8
        color: ui.theme.backgroundSecondaryColor
        border.color: ui.theme.strokeColor

        Item {
            id: plotContainer

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: buttonsRow.top
            anchors.margins: 16

            FilterCurveEqGridLines {
                id: gridLines

                anchors.fill: parent

                dbMin: filterCurveEq.dbMin
                dbMax: filterCurveEq.dbMax
                loFreq: filterCurveEq.loFreq
                hiFreq: filterCurveEq.hiFreq
                linFreqScale: filterCurveEq.linFreqScale
                linesVisible: filterCurveEq.gridlinesVisible
            }

            Item {
                // Clips the plot so that only the currently-visible dB band is shown.
                // The PolylinePlot itself is sized for the full hard range, then
                // scaled & offset so the [dbMin, dbMax] window aligns with this clip.
                id: plotClip

                x: gridLines.plotX
                y: gridLines.plotY
                width: gridLines.plotW
                height: gridLines.plotH
                clip: true

                readonly property real dbVisibleSpan: filterCurveEq.dbMax - filterCurveEq.dbMin
                readonly property real dbHardSpan: filterCurveEq.dbHardMax - filterCurveEq.dbHardMin

                PolylinePlot {
                    id: curve

                    width: plotClip.width
                    height: plotClip.dbVisibleSpan > 0 ? plotClip.height * plotClip.dbHardSpan / plotClip.dbVisibleSpan : plotClip.height
                    x: 0
                    y: plotClip.dbVisibleSpan > 0 ? -plotClip.height * (filterCurveEq.dbHardMax - filterCurveEq.dbMax) / plotClip.dbVisibleSpan : 0

                    lineColor: ui.theme.accentColor
                    lineWidth: 2

                    pointRadius: 4.0
                    pointOutlineColor: ui.theme.accentColor
                    pointCentreColor: ui.theme.accentColor
                    pointOutlineWidth: 2.0

                    ghostPointRadius: 3.0
                    ghostPointOutlineColor: ui.theme.accentColor

                    drawBackground: false

                    points: filterCurveEq.curveModel.points
                    defaultValue: filterCurveEq.curveModel.defaultValue

                    xRangeFrom: 0.0
                    xRangeTo: 1.0

                    yRangeFrom: filterCurveEq.dbHardMin
                    yRangeTo: filterCurveEq.dbHardMax
                    yAxisInverse: true

                    Component.onCompleted: {
                        curve.init()
                    }

                    function activePointFreq() {
                        const norm = curve.width > 0 ? (curve.activePointX / curve.width) : 0
                        if (filterCurveEq.linFreqScale) {
                            return norm * filterCurveEq.hiFreq
                        }
                        const loLog = Math.log(filterCurveEq.loFreq) / Math.LN10
                        const hiLog = Math.log(filterCurveEq.hiFreq) / Math.LN10
                        return Math.pow(10, norm * (hiLog - loLog) + loLog)
                    }

                    onPointMoved: function (index, x, y, completed) {
                        filterCurveEq.curveModel.setPoint(index, x, y, completed)
                        tooltip.gain = y
                        tooltip.freq = curve.activePointFreq()
                        tooltip.show(true)
                    }

                    onPointAdded: function (x, y, completed) {
                        filterCurveEq.curveModel.addPoint(x, y, completed)
                    }

                    onPointRemoved: function (index, completed) {
                        filterCurveEq.curveModel.removePoint(index, completed)
                    }

                    onDragCancelled: {
                        filterCurveEq.curveModel.cancelDrag()
                        tooltip.hide(true)
                    }

                    onInteractionFinished: function () {
                        if (!curve.hasActivePoint) {
                            tooltip.hide(true)
                        }
                    }

                    onActivePointChanged: {
                        if (curve.hasActivePoint) {
                            fake.x = curve.activePointX
                            fake.y = curve.activePointY - (curve.pointRadius + 2)
                            tooltip.gain = curve.activePointValue
                            tooltip.freq = curve.activePointFreq()
                            tooltip.show(true)
                        } else {
                            tooltip.hide(true)
                        }
                    }

                    Item {
                        // fakeItem the tooltip popup is anchored to.
                        id: fake

                        width: 1
                        height: 1
                        enabled: false  // don't steal mouse events

                        FilterCurveEqTooltip {
                            id: tooltip
                        }
                    }
                }
            }
        }

        Row {
            id: buttonsRow

            spacing: 8
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 16

            FlatButton {
                id: zoomOutButton

                width: 28
                height: 28

                navigation.panel: root.buttonsNavigationPanel
                navigation.order: 0

                text: "-"
                enabled: filterCurveEq.canZoomOut

                onClicked: filterCurveEq.zoomOut()
            }

            FlatButton {
                id: zoomInButton

                width: 28
                height: 28

                navigation.panel: root.buttonsNavigationPanel
                navigation.order: zoomOutButton.navigation.order + 1

                text: "+"
                enabled: filterCurveEq.canZoomIn

                onClicked: filterCurveEq.zoomIn()
            }

            FlatButton {
                id: flattenButton

                width: 64
                height: 28

                navigation.panel: root.buttonsNavigationPanel
                navigation.order: zoomInButton.navigation.order + 1

                text: qsTrc("effects/filtercurveeq", "Flatten")

                onClicked: filterCurveEq.curveModel.flatten()
            }

            FlatButton {
                id: invertButton

                width: 64
                height: 28

                navigation.panel: root.buttonsNavigationPanel
                navigation.order: flattenButton.navigation.order + 1

                text: qsTrc("effects/filtercurveeq", "Invert")

                onClicked: filterCurveEq.curveModel.invert()
            }

            CheckBox {
                id: linFreqScaleCheckBox

                anchors.verticalCenter: parent.verticalCenter

                navigation.panel: root.buttonsNavigationPanel
                navigation.order: invertButton.navigation.order + 1

                text: qsTrc("effects/filtercurveeq", "Linear Frequency Scale")

                checked: filterCurveEq.linFreqScale

                onClicked: filterCurveEq.linFreqScale = !filterCurveEq.linFreqScale
            }

            CheckBox {
                id: showGridlinesCheckBox

                anchors.verticalCenter: parent.verticalCenter

                navigation.panel: root.buttonsNavigationPanel
                navigation.order: linFreqScaleCheckBox.navigation.order + 1

                text: qsTrc("effects/filtercurveeq", "Show grid lines")

                checked: filterCurveEq.gridlinesVisible

                onClicked: filterCurveEq.gridlinesVisible = !filterCurveEq.gridlinesVisible
            }
        }
    }
}

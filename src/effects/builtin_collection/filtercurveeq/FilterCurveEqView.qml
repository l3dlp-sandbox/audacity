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
                linesVisible: filterCurveEq.gridlinesVisible
            }

            PolylinePlot {
                id: curve

                x: gridLines.plotX
                y: gridLines.plotY
                width: gridLines.plotW
                height: gridLines.plotH

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

                yRangeFrom: filterCurveEq.dbMin
                yRangeTo: filterCurveEq.dbMax
                yAxisInverse: true

                Component.onCompleted: {
                    curve.init()
                }

                onPointMoved: function(index, x, y, completed) {
                    filterCurveEq.curveModel.setPoint(index, x, y, completed)
                }

                onPointAdded: function(x, y, completed) {
                    filterCurveEq.curveModel.addPoint(x, y, completed)
                }

                onPointRemoved: function(index, completed) {
                    filterCurveEq.curveModel.removePoint(index, completed)
                }

                onDragCancelled: {
                    filterCurveEq.curveModel.cancelDrag()
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
                id: flattenButton

                width: 64
                height: 28

                navigation.panel: root.buttonsNavigationPanel
                navigation.order: 0

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
                id: showGridlinesCheckBox

                anchors.verticalCenter: parent.verticalCenter

                navigation.panel: root.buttonsNavigationPanel
                navigation.order: invertButton.navigation.order + 1

                text: qsTrc("effects/filtercurveeq", "Show grid lines")

                checked: filterCurveEq.gridlinesVisible

                onClicked: filterCurveEq.gridlinesVisible = !filterCurveEq.gridlinesVisible
            }
        }
    }
}

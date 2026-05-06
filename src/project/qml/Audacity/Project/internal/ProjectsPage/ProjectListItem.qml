/*
* Audacity: A Digital Audio Editor
*/
import QtQuick 2.15
import QtQuick.Layouts 1.15

import Muse.Ui 1.0
import Muse.UiComponents
import Muse.GraphicalEffects 1.0
import Audacity.Project 1.0

ListItemBlank {
    id: root

    required property var item
    property var columns: []
    property real columnsContentX: 0
    property real columnsMinWidth: 0

    property real itemInset: 12
    property real columnSpacing: 44

    property alias showBottomBorder: bottomBorder.visible

    implicitHeight: 64

    navigation.accessible.name: root.item.name ?? ""
    navigation.onActiveChanged: {
        if (navigation.active) {
            root.scrollIntoView()
        }
    }

    focusBorder.anchors.bottomMargin: bottomBorder.visible ? bottomBorder.height : 0

    RowLayout {
        anchors.fill: parent

        anchors.leftMargin: root.itemInset
        anchors.rightMargin: root.itemInset

        spacing: root.columnSpacing

        // First column — fixed width, never clipped
        Loader {
            readonly property var columnData: root.columns.length > 0 ? root.columns[0] : null

            active: columnData !== null

            Layout.preferredWidth: columnData ? columnData.width : 0

            readonly property ProjectListItem listItem: root
            readonly property var item: root.item
            readonly property NavigationPanel navigationPanel: root.navigation.panel
            readonly property int navigationRow: root.navigation.row
            readonly property int navigationColumnStart: 100

            sourceComponent: columnData ? columnData.delegate : null
        }

        SeparatorLine {
            orientation: Qt.Vertical
        }

        // Remaining columns — clipped, offset follows header scroll
        Item {
            Layout.preferredHeight: parent.height
            Layout.fillWidth: true
            clip: true

            RowLayout {
                id: columnsRow
                x: -root.columnsContentX
                width: Math.max(parent.width, root.columnsMinWidth)
                height: parent.height

                spacing: root.columnSpacing

                Repeater {
                    id: columnsRepeater

                    model: Math.max(root.columns.length - 1, 0)

                    delegate: Loader {
                        readonly property var columnData: root.columns[model.index + 1]

                        Layout.preferredWidth: columnData.width
                        Layout.minimumWidth: columnData.width
                        Layout.fillWidth: columnData.fillWidth

                        readonly property ProjectListItem listItem: root
                        readonly property var item: root.item
                        readonly property NavigationPanel navigationPanel: root.navigation.panel
                        readonly property int navigationRow: root.navigation.row
                        readonly property int navigationColumnStart: 100 * (model.index + 2)

                        sourceComponent: columnData.delegate
                    }
                }
            }
        }
    }

    SeparatorLine {
        id: bottomBorder
        anchors.bottom: parent.bottom
    }
}

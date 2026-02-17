import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Muse.Ui
import Muse.UiComponents

Item {
    id: root

    property var model: null

    property NavigationPanel navigation: NavigationPanel {
        name: "GetEffectsMenuPanel"
        enabled: root.enabled && root.visible
        direction: NavigationPanel.Vertical
        onActiveChanged: function (active) {
            if (active) {
                root.forceActiveFocus()
            }
        }

        onNavigationEvent: function (event) {
            if (event.type === NavigationEvent.AboutActive) {
                event.setData("controlIndex", [listView.currentIndex])
            }
        }
    }

    QtObject {
        id: prv

        readonly property int spaceS: 4
        readonly property int spaceM: 8
        readonly property int spaceL: 12
        readonly property int spaceXL: 16
        readonly property int spaceXXL: 24

        readonly property int tabHeight: 32
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: ui.theme.backgroundPrimaryColor
    }

    ListView {
        id: listView

        anchors.fill: parent
        anchors.topMargin: prv.spaceM
        anchors.bottomMargin: prv.spaceM

        clip: true
        spacing: 0
        model: root.model ? root.model.categories : []

        ScrollBar.vertical: StyledScrollBar {}

        delegate: PageTabButton {
            width: listView.width
            height: prv.tabHeight

            orientation: Qt.Horizontal
            spacing: prv.spaceM
            leftPadding: prv.spaceL

            normalStateFont: ui.theme.bodyFont
            selectedStateFont: ui.theme.bodyBoldFont

            title: modelData
            checked: root.model ? (index === root.model.selectedCategoryIndex) : false

            navigation.name: "GetEffectsMenuItem"
            navigation.panel: root.navigation
            navigation.row: index
            navigation.accessible.name: title
            navigation.accessible.role: MUAccessible.ListItem

            onClicked: {
                if (root.model) {
                    root.model.selectedCategoryIndex = index
                }
            }
        }
    }
}

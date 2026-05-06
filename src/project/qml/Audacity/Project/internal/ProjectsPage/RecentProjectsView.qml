/*
* Audacity: A Digital Audio Editor
*/
import QtQuick 2.15
import QtQuick.Controls 2.15

import Muse.Ui 1.0
import Muse.UiComponents
import Audacity.Project 1.0

ProjectsView {
    id: root

    RecentProjectsModel {
        id: recentProjectsModel
    }

    Component.onCompleted: {
        recentProjectsModel.load()
    }

    sourceComponent: root.viewType === ProjectsPageModel.List ? listComp : gridComp

    QtObject {
        id: prv
        property string placeholderFile: ":/resources/ProjectPlaceholder.svg"
    }

    Component {
        id: cloudIndicatorComp

        CloudProjectIndicatorButton {
            mouseArea.enabled: false

            isProgress: false
            isDownloadedAndUpToDate: true
        }
    }

    Component {
        id: gridComp

        ProjectsGridView {
            anchors.fill: parent

            model: recentProjectsModel
            searchText: root.searchText

            backgroundColor: root.backgroundColor
            sideMargin: root.sideMargin
            placeholder: prv.placeholderFile

            indicatorButton: cloudIndicatorComp

            navigation.section: root.navigationSection
            navigation.order: root.navigationOrder
            navigation.name: "RecentProjectsGrid"
            navigation.accessible.name: qsTrc("project", "Recent projects grid")

            onCreateNewProjectRequested: {
                root.createNewProjectRequested()
            }

            onOpenProjectRequested: function (projectPath, displayName) {
                root.openProjectRequested(projectPath, displayName)
            }

            onOpenCloudProjectRequested: function (projectId, projectPath, displayName) {
                root.openCloudProjectRequested(projectId, projectPath, displayName)
            }
        }
    }

    Component {
        id: listComp

        ProjectsListView {
            id: list

            readonly property int nameColumnWidth: 200
            readonly property int thumbnailColumnWidth: 200
            readonly property int iconColumnWidth: 48
            readonly property int modifiedColumnWidth: 100
            readonly property int sizeColumnWidth: 75
            readonly property int btnColumnWidth: 44

            anchors.fill: parent

            model: recentProjectsModel
            searchText: root.searchText

            backgroundColor: root.backgroundColor
            sideMargin: root.sideMargin

            showNewProjectItem: true

            navigation.section: root.navigationSection
            navigation.order: root.navigationOrder
            navigation.name: "RecentProjectsList"
            navigation.accessible.name: qsTrc("project", "Recent projects list")

            onCreateNewProjectRequested: {
                root.createNewProjectRequested()
            }

            onOpenProjectRequested: function (projectPath, displayName) {
                root.openProjectRequested(projectPath, displayName)
            }

            onOpenCloudProjectRequested: function (projectId, projectPath, displayName) {
                root.openCloudProjectRequested(projectId, projectPath, displayName)
            }

            columns: [
                ProjectsListView.ColumnItem {
                    id: nameColumn

                    header: qsTrc("project", "Name")

                    width: nameColumnWidth

                    delegate: StyledTextLabel {
                        id: nameLabel

                        text: item.name ?? ""
                        font: ui.theme.largeBodyFont
                        horizontalAlignment: Text.AlignLeft

                        NavigationFocusBorder {
                            navigationCtrl: NavigationControl {
                                name: "NameLabel"
                                panel: navigationPanel
                                row: navigationRow
                                column: navigationColumnStart
                                enabled: nameLabel.visible && nameLabel.enabled && !nameLabel.isEmpty
                                accessible.name: nameColumn.header + ": " + nameLabel.text
                                accessible.role: MUAccessible.StaticText

                                onActiveChanged: {
                                    if (active) {
                                        listItem.scrollIntoView()
                                    }
                                }
                            }
                        }
                    }
                },
                ProjectsListView.ColumnItem {
                    id: thumbnailColumn

                    header: ""

                    width: thumbnailColumnWidth
                    fillWidth: true

                    delegate: Row {
                        spacing: 0

                        ProjectThumbnail {
                            id: thumbnail

                            height: 48
                            width: 90

                            anchors.verticalCenter: parent.verticalCenter

                            path: item.thumbnailUrl ?? ""
                            placeholder: prv.placeholderFile

                            backgroundColor: ui.theme.backgroundSecondaryColor
                            lineColor: Qt.alpha(ui.theme.fontPrimaryColor, 0.8)
                            borderColor: ui.theme.strokeColor
                        }

                        Item {
                            width: parent.width - thumbnail.width
                            height: parent.height
                        }
                    }
                },
                ProjectsListView.ColumnItem {
                    id: cloudIndicatorColumn

                    header: ""

                    width: iconColumnWidth

                    delegate: Item {
                        readonly property bool isCloud: item.isCloud ?? false

                        width: parent.width
                        height: parent.height

                        Loader {
                            active: isCloud
                            anchors.centerIn: parent

                            sourceComponent: CloudProjectIndicatorButton {
                                isProgress: false
                                isDownloadedAndUpToDate: true

                                NavigationFocusBorder {
                                    navigationCtrl: NavigationControl {
                                        name: "CloudIndicator"
                                        panel: navigationPanel
                                        row: navigationRow
                                        column: navigationColumnStart
                                        enabled: isCloud
                                        accessible.name: qsTrc("project", "Cloud project indicator")
                                        accessible.role: MUAccessible.Information

                                        onActiveChanged: {
                                            if (active) {
                                                listItem.scrollIntoView()
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                },
                ProjectsListView.ColumnItem {
                    id: modifiedColumn

                    header: qsTrc("project", "Modified")

                    width: modifiedColumnWidth

                    delegate: StyledTextLabel {
                        id: modifiedLabel
                        text: item.timeSinceModified ?? ""

                        font.capitalization: Font.AllUppercase
                        horizontalAlignment: Text.AlignLeft

                        NavigationFocusBorder {
                            navigationCtrl: NavigationControl {
                                name: "ModifiedLabel"
                                panel: navigationPanel
                                row: navigationRow
                                column: navigationColumnStart
                                enabled: modifiedLabel.visible && modifiedLabel.enabled && !modifiedLabel.isEmpty
                                accessible.name: modifiedColumn.header + ": " + modifiedLabel.text
                                accessible.role: MUAccessible.StaticText

                                onActiveChanged: {
                                    if (active) {
                                        listItem.scrollIntoView()
                                    }
                                }
                            }

                            anchors.margins: -radius
                            radius: 2 + border.width
                        }
                    }
                },
                ProjectsListView.ColumnItem {
                    id: sizeColumn

                    header: qsTrc("global", "Size", "file size")

                    width: sizeColumnWidth

                    delegate: StyledTextLabel {
                        id: sizeLabel
                        text: Boolean(item.fileSize) ? item.fileSize : "-"

                        font: ui.theme.largeBodyFont
                        horizontalAlignment: Text.AlignLeft

                        NavigationFocusBorder {
                            navigationCtrl: NavigationControl {
                                name: "SizeLabel"
                                panel: navigationPanel
                                row: navigationRow
                                column: navigationColumnStart
                                enabled: sizeLabel.visible && sizeLabel.enabled && !sizeLabel.isEmpty
                                accessible.name: sizeColumn.header + ": " + (Boolean(item.fileSize) ? item.fileSize : qsTrc("global", "Unknown"))
                                accessible.role: MUAccessible.StaticText

                                onActiveChanged: {
                                    if (active) {
                                        listItem.scrollIntoView()
                                    }
                                }
                            }

                            anchors.margins: -radius
                            radius: 2 + border.width
                        }
                    }
                },
                ProjectsListView.ColumnItem {
                    id: btnColumn

                    header: ""

                    width: btnColumnWidth

                    delegate: Item {
                        width: parent.width
                        height: 28

                        MenuButton {
                            id: menuButton

                            visible: Boolean(item.contextMenuModel)

                            width: 28
                            height: 28

                            anchors.centerIn: parent

                            menuModel: item.contextMenuModel

                            onHandleMenuItem: function (itemId) {
                                item.contextMenuModel.handleMenuItem(itemId)
                            }

                            Component.onCompleted: {
                                if (item.contextMenuModel != null) {
                                    item.contextMenuModel.load()
                                }
                            }

                            Rectangle {
                                anchors.fill: parent
                                color: "transparent"
                                radius: 3
                                border.width: 1
                                border.color: ui.theme.strokeColor
                            }

                            NavigationFocusBorder {
                                navigationCtrl: NavigationControl {
                                    name: "MenuButton"
                                    panel: navigationPanel
                                    row: navigationRow
                                    column: navigationColumnStart
                                    enabled: menuButton.visible && menuButton.enabled
                                    accessible.name: qsTrc("project", "Project item menu")
                                    accessible.role: MUAccessible.Button

                                    onActiveChanged: {
                                        if (active) {
                                            listItem.scrollIntoView()
                                        }
                                    }

                                    onTriggered: {
                                        menuButton.clicked(null)
                                    }
                                }
                            }
                        }
                    }
                }
            ]
        }
    }
}

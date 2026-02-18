/*
 * Audacity: A Digital Audio Editor
 */
import QtQuick
import QtQuick.Layouts

import Muse.Ui
import Muse.UiComponents
import Muse.Vst

import Audacity.Effects
import Audacity.BuiltinEffects
import Audacity.Lv2
import Audacity.AudioUnit
import Audacity.Vst

EffectStyledDialogView {
    id: root

    property alias effectState: viewerModel.effectState

    title: viewerModel.title + " - " + viewerModel.trackName
    navigationSection.name: title

    contentWidth: Math.max(viewerLoader.width, prv.minimumWidth)
    contentHeight: 2 * prv.padding + viewerLoader.height + presetsBar.height

    alwaysOnTop: true

    QtObject {
        id: prv
        property int minimumWidth: viewerModel.effectFamily === EffectFamily.LV2 ? 500 : 270
        property int padding: viewerModel.effectFamily == EffectFamily.Builtin ? 16 : 4
        property alias viewItem: viewerLoader.item
    }

    Component.onCompleted: {
        viewerModel.load()
        loadViewer()
    }

    // Listen to UI mode changes from the presets bar menu
    Connections {
        target: presetsBar.manageMenuModel
        function onUseVendorUIChanged() {
            viewerModel.refreshUIMode()
        }
    }

    Connections {
        target: viewerModel
        function onViewerComponentTypeChanged() {
            // For Audio Units, reload the view instead of switching components
            if (viewerModel.viewerComponentType === ViewerComponentType.AudioUnit && viewerLoader.item) {
                viewerLoader.item.reload()
            } else {
                loadViewer()
            }
        }
    }

    function loadViewer() {
        switch (viewerModel.viewerComponentType) {
        case ViewerComponentType.AudioUnit:
            viewerLoader.sourceComponent = audioUnitViewerComponent
            break
        case ViewerComponentType.Lv2:
            viewerLoader.sourceComponent = lv2ViewerComponent
            break
        case ViewerComponentType.Vst:
            viewerLoader.sourceComponent = vstViewerComponent
            break
        case ViewerComponentType.Builtin:
            viewerLoader.sourceComponent = builtinViewerComponent
            break
        case ViewerComponentType.Generated:
            viewerLoader.sourceComponent = generatedViewerComponent
            break
        default:
            viewerLoader.sourceComponent = null
        }
    }

    RealtimeEffectViewerDialogModel {
        id: viewerModel
        effectState: root.effectState
        dialogView: root
        navigationPanel: root.navigationPanel
    }

    Component {
        id: audioUnitViewerComponent
        AudioUnitViewer {
            instanceId: root.instanceId
            topPadding: headerBar.y + headerBar.height + prv.padding
            minimumWidth: prv.minimumWidth
        }
    }

    Component {
        id: lv2ViewerComponent
        Lv2Viewer {
            instanceId: root.instanceId
            effectState: root.effectState
            title: root.title
        }
    }

    Component {
        id: vstViewerComponent
        VstViewer {
            instanceId: root.instanceId
            topPadding: headerBar.y + headerBar.height + prv.padding
            minimumWidth: prv.minimumWidth
        }
    }

    Component {
        id: builtinViewerComponent
        Column {
            topPadding: 0
            leftPadding: prv.padding
            rightPadding: prv.padding
            bottomPadding: prv.padding

            property alias instanceId: viewer.instanceId

            BuiltinEffectViewer {
                id: viewer
                instanceId: root.instanceId
                usedDestructively: false
            }
        }
    }

    Component {
        id: generatedViewerComponent
        GeneratedEffectViewer {
            id: view
            instanceId: root.instanceId
        }
    }

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        WindowContainer {
            Layout.fillWidth: true

            window: Window {
                id: win

                color: ui.theme.backgroundPrimaryColor

                height: headerBar.implicitHeight + prv.padding * 2
                width: headerBar.implicitWidth

                RowLayout {
                    id: headerBar
                    anchors.fill: parent
                    anchors.margins: prv.padding
                    spacing: presetsBar.spacing

                    BypassEffectButton {
                        id: bypassBtn

                        navigation.panel: root.navigationPanel
                        navigation.order: 0
                        navigation.name: "Bypass effect"
                        size: presetsBar.implicitHeight
                        isMasterEffect: viewerModel.isMasterEffect
                        accentButton: viewerModel.isActive

                        onClicked: viewerModel.isActive = !viewerModel.isActive
                    }

                    EffectPresetsBar {
                        id: presetsBar

                        parentWindow: root.window
                        navigationPanel: root.navigationPanel
                        navigationOrder: 1
                        instanceId: root.instanceId
                        Layout.fillWidth: true
                    }
                }
            }
        }
        Loader {
            id: viewerLoader
        }
    }
}

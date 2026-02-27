import QtQuick

import Audacity.Spectrogram

Rectangle {
    id: root

    color: "gray"

    required property int trackId
    signal zoomChanged

    SpectrogramChannelRulerModel {
        id: rulerModel
        trackId: root.trackId
        channelHeight: root.height
        onZoomChanged: root.zoomChanged()
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.NoButton

        onWheel: function (wheelEvent) {
            if (wheelEvent.modifiers & Qt.ControlModifier) {
                wheelEvent.accepted = true
                if (wheelEvent.angleDelta.y > 0) {
                    rulerModel.zoomIn(wheelEvent.y)
                } else if (wheelEvent.angleDelta.y < 0) {
                    rulerModel.zoomOut(wheelEvent.y)
                }
            }
        }
    }
}

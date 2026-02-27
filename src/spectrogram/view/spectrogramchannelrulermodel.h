/*
 * Audacity: A Digital Audio Editor
 */
#pragma once

#include "spectrogram/ispectrogramservice.h"

#include "modularity/ioc.h"

#include <QObject>

namespace au::spectrogram {
class SpectrogramChannelRulerModel : public QObject, public muse::Injectable
{
    Q_OBJECT

    Q_PROPERTY(int trackId READ trackId WRITE setTrackId NOTIFY trackIdChanged FINAL)
    Q_PROPERTY(double channelHeight READ channelHeight WRITE setChannelHeight NOTIFY channelHeightChanged FINAL)

    muse::Inject<ISpectrogramService> spectrogramService{ this };

public:
    SpectrogramChannelRulerModel(QObject* parent = nullptr);

    int trackId() const;
    void setTrackId(int trackId);

    double channelHeight() const;
    void setChannelHeight(double height);

    Q_INVOKABLE void zoomIn(double mouseY);
    Q_INVOKABLE void zoomOut(double mouseY);

signals:
    void trackIdChanged();
    void channelHeightChanged();
    void zoomChanged();

private:
    void zoomBy(double factor, double centerPosition);
    double frequencyToPosition(double freq) const;
    double positionToFrequency(double pos) const;

    int m_trackId = -1;
    double m_channelHeight = 0.0;
};
} // namespace au::spectrogram

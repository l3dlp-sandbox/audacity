/*
 * Audacity: A Digital Audio Editor
 */
#include "spectrogramchannelrulermodel.h"

#include "framework/global/types/number.h"
#include "framework/global/log.h"

namespace au::spectrogram {
SpectrogramChannelRulerModel::SpectrogramChannelRulerModel(QObject* parent)
    : QObject(parent), muse::Injectable(muse::iocCtxForQmlObject(this))
{
}

int SpectrogramChannelRulerModel::trackId() const
{
    return m_trackId;
}

void SpectrogramChannelRulerModel::setTrackId(int trackId)
{
    if (m_trackId == trackId) {
        return;
    }
    m_trackId = trackId;
    emit trackIdChanged();
}

double SpectrogramChannelRulerModel::channelHeight() const
{
    return m_channelHeight;
}

void SpectrogramChannelRulerModel::setChannelHeight(double height)
{
    if (muse::is_equal(m_channelHeight, height)) {
        return;
    }
    m_channelHeight = height;
    emit channelHeightChanged();
}

namespace {
constexpr double ZOOM_FACTOR = 1.4142;     // sqrt(2)
}

void SpectrogramChannelRulerModel::zoomIn(double mouseY)
{
    zoomBy(1. / ZOOM_FACTOR, mouseY);
}

void SpectrogramChannelRulerModel::zoomOut(double mouseY)
{
    zoomBy(ZOOM_FACTOR, mouseY);
}

double SpectrogramChannelRulerModel::frequencyToPosition(double freq) const
{
    return spectrogramService()->frequencyToY(trackId(), freq, m_channelHeight);
}

double SpectrogramChannelRulerModel::positionToFrequency(double pos) const
{
    return spectrogramService()->yToFrequency(trackId(), pos, m_channelHeight);
}

void SpectrogramChannelRulerModel::zoomBy(double factor, double mousePos)
{
    const auto config = spectrogramService()->trackSpectrogramConfiguration(m_trackId);
    IF_ASSERT_FAILED(config) {
        return;
    }

    if (config->maxFreq() > config->minFreq()) {
        // We do the calculations in the "positional" domain, where equal y steps mean equal steps on the chosen
        // spectrogram scale (e.g. on the mel scale, 1 pixel may equal 0.1 mel).

        // Hard maximum frequency -> "hard minimum" position (frequency and positional domains have opposite polarities).
        const auto hardMinPos = frequencyToPosition(spectrogramService()->frequencyHardMaximum(m_trackId));

        // If the mouse is say at 300px on a 400px ruler, the space above the mouse will change from 300 to 300 * factor.
        const auto newSpaceAbove = mousePos * factor;

        // Do as though we displaced top and bottom from [0, height] to [newBottom, newTop].
        // Then calculate the corresponding frequencies. When setting them as min and max frequencies on the configuration,
        // the spectrogram and ruler painting will rescale their content so that this new range fits in [0, height] again.

        // Don't forget that the spectrogram display of a given track must be upper-bound to the track's frequency.
        const auto newTop = std::max(hardMinPos, mousePos - newSpaceAbove);
        const auto newBottom = newTop + m_channelHeight * factor;

        const auto newMaxFreq = positionToFrequency(newTop);
        const auto newMinFreq = std::max(0., positionToFrequency(newBottom));

        config->setMaxFreq(newMaxFreq);
        config->setMinFreq(newMinFreq);
    } else {
        // Special case: we are showing a spectrum slice and all positions map to the same frequency.
        // Workaround: augment the range to 1Hz.
        const auto newMinFreq = std::max(0, config->minFreq() - 1);
        config->setMinFreq(newMinFreq);
        config->setMaxFreq(newMinFreq + 1);
    }

    // Changing track spectrogram setting decouples it from the global spectrogram settings.
    config->setUseGlobalSettings(false);

    emit zoomChanged();
}
}

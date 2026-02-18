/*
* Audacity: A Digital Audio Editor
*/
#pragma once

#include "ifrequencyselectioncontroller.h"

namespace au::spectrogram {
class FrequencySelectionController : public IFrequencySelectionController, public muse::Injectable
{
public:
    FrequencySelectionController(const muse::modularity::ContextPtr& ctx);

    spectrogram::FrequencySelection frequencySelection() const override;
    void setFrequencySelection(spectrogram::FrequencySelection) override;
    void resetFrequencySelection() override;
    muse::async::Channel<int /*track id*/> frequencySelectionChanged() const override;

private:
    FrequencySelection m_frequencySelection;
    muse::async::Channel<int> m_frequencySelectionChanged;
};
}

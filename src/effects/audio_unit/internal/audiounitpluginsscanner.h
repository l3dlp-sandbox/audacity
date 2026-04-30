/*
* Audacity: A Digital Audio Editor
*/
#pragma once

#include "modularity/ioc.h"

#include "effects/effects_base/internal/au3/au3audiopluginscanner.h"
#include "effects/effects_base/ieffectsconfiguration.h"

#include "au3-audio-unit/AudioUnitEffectsModule.h"

namespace au::effects {
class AudioUnitPluginsScanner : public Au3AudioPluginScanner
{
    muse::GlobalInject<effects::IEffectsConfiguration> effectsConfiguration;

public:
    AudioUnitPluginsScanner()
        : Au3AudioPluginScanner(m_auModule) {}

protected:
    muse::io::paths_t customPaths() const override
    {
        return effectsConfiguration()->auCustomPaths();
    }

private:
    ::AudioUnitEffectsModule m_auModule;
};
}

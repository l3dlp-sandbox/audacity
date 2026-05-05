/*
* Audacity: A Digital Audio Editor
*/
#pragma once

#include "modularity/ioc.h"

#include "effects/effects_base/internal/au3/au3audiopluginscanner.h"
#include "effects/effects_base/ieffectsconfiguration.h"

#include "au3-module-manager/PluginManager.h"
#include "au3-vst3/VST3EffectsModule.h"

namespace au::effects {
class Vst3PluginsScanner : public Au3AudioPluginScanner
{
    muse::GlobalInject<effects::IEffectsConfiguration> effectsConfiguration;

public:
    Vst3PluginsScanner()
        : Au3AudioPluginScanner(m_vst3Module) {}

protected:
    muse::io::paths_t customPaths() const override
    {
        return effectsConfiguration()->vst3CustomPaths();
    }

    ::PluginPaths pluginPaths() const override
    {
        BasicUI::ProgressDialog* progress = progressDialog();
        if (!progress) {
            return Au3AudioPluginScanner::pluginPaths();
        }
        return m_vst3Module.FindModulePaths(::PluginManager::Get(), progress);
    }

private:
    ::VST3EffectsModule m_vst3Module;
};
}

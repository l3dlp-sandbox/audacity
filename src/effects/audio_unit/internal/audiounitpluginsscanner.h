/*
* Audacity: A Digital Audio Editor
*/
#pragma once

#include "effects/effects_base/internal/au3/au3audiopluginscanner.h"

#include "au3-audio-unit/AudioUnitEffectsModule.h"
#include "au3-module-manager/PluginManager.h"

namespace au::effects {
class AudioUnitPluginsScanner : public Au3AudioPluginScanner
{
public:
    AudioUnitPluginsScanner()
        : Au3AudioPluginScanner(m_auModule) {}

protected:
    ::PluginPaths pluginPaths() const override
    {
        BasicUI::ProgressDialog* progress = progressDialog();
        if (!progress) {
            return Au3AudioPluginScanner::pluginPaths();
        }
        return m_auModule.FindModulePaths(::PluginManager::Get(), progress);
    }

private:
    ::AudioUnitEffectsModule m_auModule;
};
}

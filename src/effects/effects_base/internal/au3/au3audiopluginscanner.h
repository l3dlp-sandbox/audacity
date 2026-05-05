/*
 * Audacity: A Digital Audio Editor
 */
#pragma once

#include "framework/audioplugins/iaudiopluginsscanner.h"
#include "framework/global/io/path.h"

#include "au3-strings/Identifier.h" // PluginPaths

class PluginProvider;
namespace BasicUI {
class ProgressDialog;
}

namespace au::effects {
class Au3AudioPluginScanner : public muse::audioplugins::IAudioPluginsScanner
{
public:

    Au3AudioPluginScanner(PluginProvider& provider);

    void init();
    void deinit();

    muse::io::paths_t scanPlugins() const override;

    void setProgressDialog(BasicUI::ProgressDialog* progress);

protected:
    virtual ::PluginPaths pluginPaths() const;
    virtual muse::io::paths_t customPaths() const { return {}; }

    PluginProvider& pluginProvider() const { return m_pluginProvider; }
    BasicUI::ProgressDialog* progressDialog() const { return m_progress; }

private:
    virtual void doInit() {}
    void syncCustomPathsToProvider() const;

    PluginProvider& m_pluginProvider;
    BasicUI::ProgressDialog* m_progress { nullptr };
};
}

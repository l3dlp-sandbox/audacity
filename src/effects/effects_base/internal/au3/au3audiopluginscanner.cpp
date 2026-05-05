/*
 * Audacity: A Digital Audio Editor
 */

#include "au3audiopluginscanner.h"

#include "au3-module-manager/PluginManager.h"

#include "au3wrap/internal/wxtypes_convert.h"

#include "framework/global/io/dir.h"

namespace au::effects {
Au3AudioPluginScanner::Au3AudioPluginScanner(PluginProvider& provider)
    : m_pluginProvider{provider}
{
}

void Au3AudioPluginScanner::init()
{
    m_pluginProvider.Initialize();
    doInit();
}

void Au3AudioPluginScanner::deinit()
{
    m_pluginProvider.Terminate();
}

void Au3AudioPluginScanner::setProgressDialog(BasicUI::ProgressDialog* progress)
{
    m_progress = progress;
}

muse::io::paths_t Au3AudioPluginScanner::scanPlugins() const
{
    // Push user-configured custom paths into PluginManager so that providers
    // (e.g. VST3) which read them via ReadCustomPaths(*this) pick them up.
    // This must NOT run during plugin-registration subprocesses
    // (`--register-audio-plugin`), which never invoke scanPlugins() and don't
    // load effects_base / IEffectsConfiguration / PluginManager::Initialize().
    syncCustomPathsToProvider();

    muse::io::paths_t result;

    const PluginPaths paths = pluginPaths();
    for (const auto& path : paths) {
        const auto modulePath = path.BeforeFirst(';');
        auto convertedPath = muse::io::Dir::fromNativeSeparators(au3::wxToString(modulePath));
        result.emplace_back(std::move(convertedPath));
    }

    return result;
}

PluginPaths Au3AudioPluginScanner::pluginPaths() const
{
    // PluginManager still needed here by some effect modules for the implementation of custom paths.
    return m_pluginProvider.FindModulePaths(PluginManager::Get());
}

void Au3AudioPluginScanner::syncCustomPathsToProvider() const
{
    const muse::io::paths_t userPaths = customPaths();
    ::PluginPaths customPathsForProvider;
    customPathsForProvider.reserve(userPaths.size());
    for (const auto& p : userPaths) {
        if (!p.empty()) {
            customPathsForProvider.emplace_back(wxString::FromUTF8(p.toStdString()));
        }
    }
    PluginManager::Get().StoreCustomPaths(m_pluginProvider, customPathsForProvider);
}
}

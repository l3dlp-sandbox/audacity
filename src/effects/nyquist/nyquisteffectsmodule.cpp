/*
* Audacity: A Digital Audio Editor
*/
#include "nyquisteffectsmodule.h"

#include "audioplugins/iaudiopluginsscannerregister.h"
#include "audioplugins/iaudiopluginmetareaderregister.h"

#include "effects/effects_base/iparameterextractorregistry.h"
#include "effects/effects_base/ieffectviewlaunchregister.h"
#include "effects/effects_base/view/effectsviewutils.h"

#include "internal/nyquisteffectsrepository.h"
#include "internal/nyquistparameterextractorservice.h"
#include "internal/nyquistviewlauncher.h"
#include "internal/nyquistpluginsscanner.h"
#include "internal/nyquistpluginsmetareader.h"

au::effects::NyquistEffectsModule::NyquistEffectsModule()
    : m_nyquistMetaReader(std::make_shared<NyquistPluginsMetaReader>())
{
}

std::string au::effects::NyquistEffectsModule::moduleName() const
{
    return "effects_nyquist";
}

void au::effects::NyquistEffectsModule::registerExports()
{
    m_nyquistEffectsRepository = std::make_shared<NyquistEffectsRepository>(muse::modularity::globalCtx());

    globalIoc()->registerExport<INyquistEffectsRepository>(moduleName(), m_nyquistEffectsRepository);
}

void au::effects::NyquistEffectsModule::resolveImports()
{
    auto scannerRegister = globalIoc()->resolve<muse::audioplugins::IAudioPluginsScannerRegister>(moduleName());
    if (scannerRegister) {
        scannerRegister->registerScanner(std::make_shared<NyquistPluginsScanner>());
    }

    auto metaReaderRegister = globalIoc()->resolve<muse::audioplugins::IAudioPluginMetaReaderRegister>(moduleName());
    if (metaReaderRegister) {
        metaReaderRegister->registerReader(m_nyquistMetaReader);
    }

    auto paramExtractorRegistry = globalIoc()->resolve<IParameterExtractorRegistry>(moduleName());
    if (paramExtractorRegistry) {
        paramExtractorRegistry->registerExtractor(std::make_shared<NyquistParameterExtractorService>());
    }

    auto launchRegister = globalIoc()->resolve<IEffectViewLaunchRegister>(moduleName());
    if (launchRegister) {
        launchRegister->regLauncher("Nyquist", std::make_shared<NyquistViewLauncher>(muse::modularity::globalCtx()));
    }
}

void au::effects::NyquistEffectsModule::onInit(const muse::IApplication::RunMode& runMode)
{
    m_nyquistMetaReader->init(runMode);
}

void au::effects::NyquistEffectsModule::onDeinit()
{
    m_nyquistMetaReader->deinit();
}

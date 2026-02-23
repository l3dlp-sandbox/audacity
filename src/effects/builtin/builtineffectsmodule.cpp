/*
* Audacity: A Digital Audio Editor
*/
#include "builtineffectsmodule.h"

#include "globalmodule.h"
#include "internal/builtineffectsloader.h"
#include "internal/builtinviewlauncher.h"

#include "common/valuewarper/valuewarper.h"

#include "effects/effects_base/ieffectviewlaunchregister.h"

using namespace au::effects;

static const std::string mname("effects_builtin");

static void effects_builtin_init_qrc()
{
    Q_INIT_RESOURCE(effects_builtin);
}

std::string BuiltinEffectsModule::moduleName() const
{
    return mname;
}

void BuiltinEffectsModule::registerExports()
{
}

void BuiltinEffectsModule::registerResources()
{
    effects_builtin_init_qrc();
}

void BuiltinEffectsModule::registerUiTypes()
{
    qmlRegisterType<ValueWarper>("Audacity.BuiltinEffects", 1, 0, "ValueWarper");
}

void BuiltinEffectsModule::onPreInit(const muse::IApplication::RunMode&)
{
    //! NOTE preInit() only creates static Registration objects (doesn't use `this`).
    //! Must run at module level before Au3WrapModule::onInit() sets sInitialized = true.
    BuiltinEffectsLoader::preInit();
}

muse::modularity::IContextSetup* BuiltinEffectsModule::newContext(const muse::modularity::ContextPtr& ctx) const
{
    return new BuiltinEffectsContext(ctx);
}

// =====================================================
// BuiltinEffectsContext
// =====================================================

BuiltinEffectsContext::BuiltinEffectsContext(const muse::modularity::ContextPtr& ctx)
    : muse::modularity::IContextSetup(ctx), m_builtinEffectsLoader(std::make_unique<BuiltinEffectsLoader>(muse::modularity::globalCtx()))
{
}

void BuiltinEffectsContext::registerExports()
{
}

void BuiltinEffectsContext::resolveImports()
{
    auto lr = ioc()->resolve<IEffectViewLaunchRegister>(mname);
    if (lr) {
        lr->regLauncher("Audacity" /*builtin*/, std::make_shared<BuiltinViewLauncher>(iocContext()));
    }
}

void BuiltinEffectsContext::onInit(const muse::IApplication::RunMode&)
{
    m_builtinEffectsLoader->init();
}

void BuiltinEffectsContext::onDeinit()
{
}

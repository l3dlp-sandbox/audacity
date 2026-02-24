/*
 * Audacity: A Digital Audio Editor
 */
#pragma once

#include "framework/global/modularity/ioc.h"
#include "effects/builtin/ieffectsviewregister.h"
#include "effects/builtin/ibuiltineffectsrepository.h"

class WaveChannel;

namespace au::effects {
class NyquistPromptLoader : public muse::Injectable
{
    muse::Inject<IEffectsViewRegister> effectsViewRegister { this };
    muse::Inject<IBuiltinEffectsRepository> builtinEffectsRepository { this };

public:

    NyquistPromptLoader(const muse::modularity::ContextPtr& ctx)
        : muse::Injectable(ctx) {}

    static void preInit();
    void init();
};
}

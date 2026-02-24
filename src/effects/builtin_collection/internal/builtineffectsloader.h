/*
* Audacity: A Digital Audio Editor
*/
#pragma once

#include "modularity/ioc.h"

#include "effects/builtin/ibuiltineffectsviewregister.h"
#include "effects/builtin/ibuiltineffectsrepository.h"
#include "modularity/ioc.h"

namespace au::effects {
class BuiltinEffectsLoader : public muse::Injectable
{
    // TODO all this should be global
    muse::Inject<IBuiltinEffectsViewRegister> builtinEffectsViewRegister { this };
    muse::Inject<IBuiltinEffectsRepository> builtinEffectsRepository { this };

public:
    BuiltinEffectsLoader(const muse::modularity::ContextPtr& ctx)
        : muse::Injectable(ctx) {}

    static void preInit();
    void init();
};
}

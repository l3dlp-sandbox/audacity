/*
 * Audacity: A Digital Audio Editor
 */
#pragma once

#include "effects/builtin/collection/common/effectsettingmodelimpl.h"
#include "limitereffect.h"

namespace au::effects {
class LimiterSettingModel : public EffectSettingModelImpl<LimiterEffect>
{
public:
    LimiterSettingModel(QObject* parent, int instanceId);
};

class LimiterSettingModelFactory : public BuiltinEffectSettingModelFactory<LimiterSettingModel>
{
};
}

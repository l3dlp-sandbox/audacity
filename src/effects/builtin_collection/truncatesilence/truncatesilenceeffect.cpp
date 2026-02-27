/*
* Audacity: A Digital Audio Editor
*/
#include "truncatesilenceeffect.h"
#include "au3-effects/LoadEffects.h"

namespace au::effects {
const ComponentInterfaceSymbol TruncateSilenceEffect::Symbol { XO("Truncate silence") };

TruncateSilenceEffect::TruncateSilenceEffect()
{
}

TruncateSilenceEffect::~TruncateSilenceEffect()
{
}

ComponentInterfaceSymbol TruncateSilenceEffect::GetSymbol() const
{
    return Symbol;
}
}

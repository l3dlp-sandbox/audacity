/*
 * Audacity: A Digital Audio Editor
 */
#include "filtercurveeqviewmodel.h"

#include "filtercurveeq.h"
#include "filtercurvemodel.h"

#include "au3-builtin-effects/EqualizationCurvesList.h"
#include "au3-builtin-effects/EqualizationFilter.h"

namespace au::effects {
FilterCurveEqViewModel::FilterCurveEqViewModel(QObject* parent, int instanceId)
    : BuiltinEffectModel{parent, instanceId},
    m_curveModel(new FilterCurveModel(this, effect<FilterCurveEq>()))
{
}

void FilterCurveEqViewModel::doReload()
{
    m_curveModel->reload();
    emit curveModelChanged();
    emit freqRangeChanged();
    emit gridlinesVisibleChanged();
}

FilterCurveModel* FilterCurveEqViewModel::curveModel() const
{
    return m_curveModel;
}

double FilterCurveEqViewModel::dbMin() const
{
    return -30.0;
}

double FilterCurveEqViewModel::dbMax() const
{
    return 30.0;
}

double FilterCurveEqViewModel::loFreq() const
{
    return effect<FilterCurveEq>().mCurvesList.mParameters.mLoFreq;
}

double FilterCurveEqViewModel::hiFreq() const
{
    return effect<FilterCurveEq>().mCurvesList.mParameters.mHiFreq;
}

bool FilterCurveEqViewModel::gridlinesVisible() const
{
    return effect<FilterCurveEq>().mCurvesList.mParameters.mDrawGrid;
}

void FilterCurveEqViewModel::setGridlinesVisible(bool v)
{
    auto& parameters = effect<FilterCurveEq>().mCurvesList.mParameters;
    if (parameters.mDrawGrid == v) {
        return;
    }
    parameters.mDrawGrid = v;
    emit gridlinesVisibleChanged();
}
}

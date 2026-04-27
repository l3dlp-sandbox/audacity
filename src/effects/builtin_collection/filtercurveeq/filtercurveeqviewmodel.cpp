/*
 * Audacity: A Digital Audio Editor
 */
#include "filtercurveeqviewmodel.h"

#include "filtercurveeq.h"
#include "filtercurvemodel.h"

#include "au3-builtin-effects/EqualizationCurvesList.h"
#include "au3-builtin-effects/EqualizationFilter.h"

#include "../equalization_common/equalizationenvelopeutils.h"

namespace au::effects {
namespace {
// Tunable: each zoom step widens or narrows the visible dB range by this
// amount on each side (so the total range changes by 2×step).
constexpr float kZoomStepDb = 6.0f;
constexpr float kMinDbHalfRange = 6.0f;    // smallest half-range allowed when zooming in
constexpr float kMaxDbHalfRange = 60.0f;   // largest half-range allowed when zooming out
}

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
    emit dbRangeChanged();
    emit linFreqScaleChanged();
}

FilterCurveModel* FilterCurveEqViewModel::curveModel() const
{
    return m_curveModel;
}

double FilterCurveEqViewModel::dbHardMin() const
{
    return -kMaxDbHalfRange;
}

double FilterCurveEqViewModel::dbHardMax() const
{
    return kMaxDbHalfRange;
}

double FilterCurveEqViewModel::dbMin() const
{
    return effect<FilterCurveEq>().mCurvesList.mParameters.mdBMin;
}

double FilterCurveEqViewModel::dbMax() const
{
    return effect<FilterCurveEq>().mCurvesList.mParameters.mdBMax;
}

bool FilterCurveEqViewModel::canZoomIn() const
{
    const auto& parameters = effect<FilterCurveEq>().mCurvesList.mParameters;
    return parameters.mdBMax - kZoomStepDb >= kMinDbHalfRange
           && -parameters.mdBMin - kZoomStepDb >= kMinDbHalfRange;
}

bool FilterCurveEqViewModel::canZoomOut() const
{
    const auto& parameters = effect<FilterCurveEq>().mCurvesList.mParameters;
    return parameters.mdBMax + kZoomStepDb <= kMaxDbHalfRange
           && -parameters.mdBMin + kZoomStepDb <= kMaxDbHalfRange;
}

void FilterCurveEqViewModel::zoomIn()
{
    if (!canZoomIn()) {
        return;
    }
    auto& parameters = effect<FilterCurveEq>().mCurvesList.mParameters;
    parameters.mdBMin += kZoomStepDb;
    parameters.mdBMax -= kZoomStepDb;
    emit dbRangeChanged();
}

void FilterCurveEqViewModel::zoomOut()
{
    if (!canZoomOut()) {
        return;
    }
    auto& parameters = effect<FilterCurveEq>().mCurvesList.mParameters;
    parameters.mdBMin -= kZoomStepDb;
    parameters.mdBMax += kZoomStepDb;
    emit dbRangeChanged();
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

bool FilterCurveEqViewModel::linFreqScale() const
{
    return effect<FilterCurveEq>().mCurvesList.mParameters.mLin;
}

void FilterCurveEqViewModel::setLinFreqScale(bool v)
{
    auto& parameters = effect<FilterCurveEq>().mCurvesList.mParameters;
    if (parameters.mLin == v) {
        return;
    }
    if (v) {
        eq_common::envLogToLin(parameters);
    } else {
        eq_common::envLinToLog(parameters);
    }
    parameters.mLin = v;
    m_curveModel->reload();
    emit linFreqScaleChanged();
    emit curveModelChanged();
}
}

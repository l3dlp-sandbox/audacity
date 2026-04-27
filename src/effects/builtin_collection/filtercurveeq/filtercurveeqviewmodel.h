/*
 * Audacity: A Digital Audio Editor
 */
#pragma once

#include "effects/builtin/view/builtineffectmodel.h"

#include "filtercurvemodel.h"

namespace au::effects {
class FilterCurveEq;

class FilterCurveEqViewModel : public BuiltinEffectModel
{
    Q_OBJECT
    Q_PROPERTY(FilterCurveModel * curveModel READ curveModel NOTIFY curveModelChanged FINAL)
    Q_PROPERTY(double dbMin READ dbMin CONSTANT FINAL)
    Q_PROPERTY(double dbMax READ dbMax CONSTANT FINAL)
    Q_PROPERTY(double loFreq READ loFreq NOTIFY freqRangeChanged FINAL)
    Q_PROPERTY(double hiFreq READ hiFreq NOTIFY freqRangeChanged FINAL)
    Q_PROPERTY(bool gridlinesVisible READ gridlinesVisible WRITE setGridlinesVisible NOTIFY gridlinesVisibleChanged FINAL)

public:
    FilterCurveEqViewModel(QObject* parent, int instanceId);

    FilterCurveModel* curveModel() const;
    double dbMin() const;
    double dbMax() const;
    double loFreq() const;
    double hiFreq() const;
    bool gridlinesVisible() const;
    void setGridlinesVisible(bool);

signals:
    void curveModelChanged();
    void freqRangeChanged();
    void gridlinesVisibleChanged();

private:
    void doReload() override;

    FilterCurveModel* const m_curveModel;
};

class FilterCurveEqViewModelFactory : public EffectViewModelFactory<FilterCurveEqViewModel>
{
};
}

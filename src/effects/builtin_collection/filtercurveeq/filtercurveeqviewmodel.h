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
    Q_PROPERTY(double dbHardMin READ dbHardMin CONSTANT FINAL)
    Q_PROPERTY(double dbHardMax READ dbHardMax CONSTANT FINAL)
    Q_PROPERTY(double dbMin READ dbMin NOTIFY dbRangeChanged FINAL)
    Q_PROPERTY(double dbMax READ dbMax NOTIFY dbRangeChanged FINAL)
    Q_PROPERTY(double loFreq READ loFreq NOTIFY freqRangeChanged FINAL)
    Q_PROPERTY(double hiFreq READ hiFreq NOTIFY freqRangeChanged FINAL)
    Q_PROPERTY(bool gridlinesVisible READ gridlinesVisible WRITE setGridlinesVisible NOTIFY gridlinesVisibleChanged FINAL)
    Q_PROPERTY(bool canZoomIn READ canZoomIn NOTIFY dbRangeChanged FINAL)
    Q_PROPERTY(bool canZoomOut READ canZoomOut NOTIFY dbRangeChanged FINAL)
    Q_PROPERTY(bool linFreqScale READ linFreqScale WRITE setLinFreqScale NOTIFY linFreqScaleChanged FINAL)

public:
    FilterCurveEqViewModel(QObject* parent, int instanceId);

    FilterCurveModel* curveModel() const;
    double dbHardMin() const;
    double dbHardMax() const;
    double dbMin() const;
    double dbMax() const;
    double loFreq() const;
    double hiFreq() const;
    bool gridlinesVisible() const;
    void setGridlinesVisible(bool);
    bool canZoomIn() const;
    bool canZoomOut() const;
    bool linFreqScale() const;
    void setLinFreqScale(bool);

    Q_INVOKABLE void zoomIn();
    Q_INVOKABLE void zoomOut();

signals:
    void curveModelChanged();
    void freqRangeChanged();
    void gridlinesVisibleChanged();
    void dbRangeChanged();
    void linFreqScaleChanged();

private:
    void doReload() override;

    FilterCurveModel* const m_curveModel;
};

class FilterCurveEqViewModelFactory : public EffectViewModelFactory<FilterCurveEqViewModel>
{
};
}

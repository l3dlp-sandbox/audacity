/*
 * Audacity: A Digital Audio Editor
 */
#include "axisticks.h"

#include "numberscale.h"

#include "framework/global/log.h"
#include "framework/global/types/number.h"

#include <algorithm>
#include <cmath>

namespace au {
namespace {
// Sorted by decreasing value.
std::vector<double> getTicksValues(double maxVal, double minVal, double step, shared::AxisScale scale)
{
    using namespace shared;

    std::vector<double> values;
    auto tick = std::floor(maxVal / step) * step;
    while (tick >= minVal) {
        if (scale == AxisScale::Logarithmic && tick <= 0) {
            break;
        }
        values.push_back(tick);
        tick -= step;
    }
    return values;
}

std::vector<shared::AxisTick> toTicks(const std::vector<double>& values, const shared::NumberScale& numberScale, double labelExtentFraction)
{
    using namespace shared;

    std::vector<AxisTick> ticks;
    ticks.reserve(values.size());
    std::transform(values.begin(), values.end(), std::back_inserter(ticks), [&numberScale](double value) {
        return AxisTick { value, static_cast<double>(numberScale.valueToPosition(static_cast<float>(value))) };
    });

    // Sort ascending by position so the overlap-rejection loop is orientation-agnostic.
    std::sort(ticks.begin(), ticks.end(), [](const AxisTick& a, const AxisTick& b) {
        return a.position < b.position;
    });

    // First and last ticks have priority over inner ticks: they are kept
    // unconditionally, and inner ticks must fit between them. Bookends are
    // edge-aligned (their labels extend a full extent into the axis rather
    // than half-and-half around the tick), matching the view's rendering.
    if (ticks.size() < 2) {
        return ticks;
    }

    auto nextTop = ticks.front().position + labelExtentFraction;
    const auto lastTop = ticks.back().position - labelExtentFraction;

    auto it = ticks.begin() + 1;
    while (it != ticks.end() - 1) {
        const auto tickTop = it->position - labelExtentFraction / 2;
        const auto tickBottom = it->position + labelExtentFraction / 2;
        if (tickTop < nextTop || tickBottom > lastTop) {
            it = ticks.erase(it);
        } else {
            nextTop = tickBottom;
            ++it;
        }
    }

    return ticks;
}

std::vector<shared::AxisTick> getTicks(double min, double max, shared::AxisScale scale, const shared::NumberScale& numberScale,
                                       double labelExtentFraction, double step)
{
    const std::vector<double> values = getTicksValues(max, min, step, scale);
    return toTicks(values, numberScale, labelExtentFraction);
}
}

shared::AxisTicks shared::axisTicks(double min, double max, AxisScale scale, double labelExtent, double axisLength,
                                    std::optional<double> majorStep)
{
    if (majorStep.has_value() && *majorStep <= 0) {
        LOGE() << "Invalid major step: " << *majorStep;
        return {};
    }

    if (axisLength <= 0) {
        LOGE() << "Invalid axis length: " << axisLength;
        return {};
    }

    const auto range = max - min;
    if (range <= 0) {
        LOGE() << "Invalid range: " << range;
        return {};
    }

    const auto safeMin = scale == AxisScale::Logarithmic ? std::max(min, 1e-6) : min;
    const auto numberScale = NumberScale{ scale, safeMin, max };

    const auto extentFraction = labelExtent / axisLength;
    if (!majorStep.has_value()) {
        majorStep = std::pow(10, std::floor(std::log10(range)));
        if (muse::is_equal(*majorStep, range)) {
            majorStep = range / 10;
        }
        // Too sparse major ticks don't look good.
        if (range / *majorStep < kMinMajorTicks) {
            *majorStep /= 2;
        }
    }

    // Major ticks: ensure min and max are bookends, on top of the on-grid values.
    auto majorValues = getTicksValues(max, safeMin, *majorStep, scale);
    if (majorValues.empty() || majorValues.front() < max) {
        majorValues.insert(majorValues.begin(), max);
    }
    if (majorValues.empty() || majorValues.back() > safeMin) {
        majorValues.push_back(safeMin);
    }
    const std::vector<AxisTick> majorTicks = toTicks(majorValues, numberScale, extentFraction);

    const auto minorStep = *majorStep / 10;
    std::vector<AxisTick> minorTicks = getTicks(safeMin, max, scale, numberScale, extentFraction, minorStep);

    // Remove minor ticks that overlap with major ticks.
    minorTicks.erase(std::remove_if(minorTicks.begin(), minorTicks.end(),
                                    [&majorTicks, extentFraction](const AxisTick& minorTick) {
        return std::any_of(majorTicks.begin(), majorTicks.end(), [&minorTick, extentFraction](const AxisTick& majorTick) {
            return std::abs(majorTick.position - minorTick.position) < extentFraction;
        });
    }),
                     minorTicks.end());

    return { majorTicks, minorTicks };
}
}

/*
 * Audacity: A Digital Audio Editor
 */
#pragma once

#include "axisscale.h"
#include "axistypes.h"

#include <optional>

namespace au::shared {
/** Minimum number of major ticks returned by `axisTicks()`. */
static constexpr auto kMinMajorTicks = 4;

/**
 * @brief Picks "good" round-number major and minor ticks across [min, max]
 *        under the given scale, dropping any that would visually overlap.
 *
 * @param min         Lower end of the data range.
 * @param max         Upper end of the data range.
 * @param scale       Scale used to map values to positions (linear, log, ...).
 * @param labelExtent Size taken up by a label along the axis. Used together
 *                    with @p axisLength to prune overlapping ticks. Must share
 *                    the same unit as @p axisLength (e.g. pixels).
 * @param axisLength  Total length of the axis in the same unit as
 *                    @p labelExtent.
 *
 * @return Major and minor ticks to display on the axis, with values and positions.
 * Major steps contain at least `kMinMajorTicks` ticks, provided that the ratio `labelExtent / axisLength` is small enough to fit them without overlap.
 */
AxisTicks axisTicks(double min, double max, AxisScale scale, double labelExtent, double axisLength);
}

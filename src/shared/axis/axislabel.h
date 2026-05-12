/*
 * Audacity: A Digital Audio Editor
 */
#pragma once

#include "axistypes.h"

#include <QString>
#include <vector>

namespace au::shared {
/**
 * @brief Builds labels for the given ticks using the smallest number of
 *        decimal digits that keeps adjacent labels distinct.
 *
 * @param ticks            Ticks to label, expected sorted by value.
 * @param maxDecimalDigits Cap on the number of decimal digits to try.
 */
std::vector<QString> labelsForTicks(const std::vector<AxisTick>& ticks, int maxDecimalDigits = 3);
}

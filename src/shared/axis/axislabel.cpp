/*
 * Audacity: A Digital Audio Editor
 */
#include "axislabel.h"

#include <algorithm>

namespace au {
namespace {
QString valueToLabel(double value, int decimalDigits)
{
    if (value >= 1000 && decimalDigits < 3) {
        auto label = QString::number(value / 1000, 'f', decimalDigits);
        // Trim trailing zeros and a trailing dot, without using regex.
        while (((label.contains('.') && (label.endsWith('0'))) || label.endsWith('.'))) {
            label.chop(1);
        }
        return label + "k";
    } else {
        return QString::number(value, 'f', decimalDigits);
    }
}
}

std::vector<QString> shared::labelsForTicks(const std::vector<AxisTick>& ticks, int maxDecimalDigits)
{
    std::vector<QString> labels;
    labels.reserve(ticks.size());
    int numDecimalDigits = 0;
    while (numDecimalDigits <= maxDecimalDigits) {
        labels.clear();
        for (const auto& tick : ticks) {
            labels.push_back(valueToLabel(tick.val, numDecimalDigits));
        }
        const auto someLabelsAreTheSame = std::adjacent_find(labels.begin(), labels.end()) != labels.end();
        if (!someLabelsAreTheSame) {
            break;
        }
        ++numDecimalDigits;
    }
    return labels;
}
}

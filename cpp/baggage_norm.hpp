#pragma once

#include <string>

namespace baggage
{

class BaggageNorm
{
public:
    BaggageNorm();
    BaggageNorm(double maxWeightKg, double maxDimsSumCm, double absoluteMaxWeightKg);

    [[nodiscard]] double GetMaxWeightKg() const noexcept
    {
        return m_maxWeightKg;
    }

    [[nodiscard]] double GetMaxDimsSumCm() const noexcept
    {
        return m_maxDimsSumCm;
    }

    [[nodiscard]] double GetAbsoluteMaxWeightKg() const noexcept
    {
        return m_absoluteMaxWeightKg;
    }

    [[nodiscard]] std::string ToString() const;

private:
    double m_maxWeightKg;
    double m_maxDimsSumCm;
    double m_absoluteMaxWeightKg;
};

} // namespace baggage

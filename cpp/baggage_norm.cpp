#include "baggage_norm.hpp"

#include <iostream>

namespace baggage
{

BaggageNorm::BaggageNorm()
    : BaggageNorm{ 23.0, 158.0, 32.0 }
{
    std::cout << "[Норма] Принята типовая норма провоза: " << ToString() << "\n";
}

BaggageNorm::BaggageNorm(double maxWeightKg, double maxDimsSumCm, double absoluteMaxWeightKg)
    : m_maxWeightKg{ maxWeightKg }
    , m_maxDimsSumCm{ maxDimsSumCm }
    , m_absoluteMaxWeightKg{ absoluteMaxWeightKg }
{
}

std::string BaggageNorm::ToString() const
{
    return "норма: " + std::to_string(m_maxWeightKg) + " кг на место, " + std::to_string(m_maxDimsSumCm)
           + " см габаритов, предельный вес " + std::to_string(m_absoluteMaxWeightKg) + " кг";
}

} // namespace baggage

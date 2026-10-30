#include "baggage_norm.hpp"

#include <iostream>
#include <sstream>

namespace
{

std::string FormatNumber(double value)
{
    std::ostringstream out;
    out << value;
    return out.str();
}

} // namespace

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
    return "норма: " + FormatNumber(m_maxWeightKg) + " кг на место, " + FormatNumber(m_maxDimsSumCm)
           + " см габаритов, предельный вес " + FormatNumber(m_absoluteMaxWeightKg) + " кг";
}

} // namespace baggage

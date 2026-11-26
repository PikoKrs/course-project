#include "fee_policies.hpp"

namespace baggage
{

double StandardFeePolicy::FeeFor(const BaseBaggage& piece, const BaggageNorm& norm) const
{
    if (piece.GetWeightKg() <= norm.GetMaxWeightKg())
    {
        return 0.0;
    }

    return (piece.GetWeightKg() - norm.GetMaxWeightKg()) * 500.0;
}

std::string StandardFeePolicy::Name() const
{
    return "базовая: 500 руб. за каждый кг перевеса";
}

double OversizeFeePolicy::FeeFor(const BaseBaggage& piece, const BaggageNorm& norm) const
{
    (void)norm;
    (void)piece;
    return 2500.0;
}

std::string OversizeFeePolicy::Name() const
{
    return "негабарит: фиксированный сбор 2500 руб.";
}

double PromoFeePolicy::FeeFor(const BaseBaggage& piece, const BaggageNorm& norm) const
{
    if (piece.GetWeightKg() <= norm.GetMaxWeightKg())
    {
        return 0.0;
    }

    return (piece.GetWeightKg() - norm.GetMaxWeightKg()) * 250.0;
}

std::string PromoFeePolicy::Name() const
{
    return "акция: 250 руб. за каждый кг перевеса (скидка 50%)";
}

} // namespace baggage

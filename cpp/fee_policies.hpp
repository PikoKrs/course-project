#pragma once

#include "i_fee_policy.hpp"

#include <string>

namespace baggage
{

class StandardFeePolicy final : public IFeePolicy
{
public:
    [[nodiscard]] double FeeFor(const BaseBaggage& piece, const BaggageNorm& norm) const override;
    [[nodiscard]] std::string Name() const override;
};

class OversizeFeePolicy final : public IFeePolicy
{
public:
    [[nodiscard]] double FeeFor(const BaseBaggage& piece, const BaggageNorm& norm) const override;
    [[nodiscard]] std::string Name() const override;
};

class PromoFeePolicy final : public IFeePolicy
{
public:
    [[nodiscard]] double FeeFor(const BaseBaggage& piece, const BaggageNorm& norm) const override;
    [[nodiscard]] std::string Name() const override;
};

} // namespace baggage

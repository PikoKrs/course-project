#pragma once

#include "base_baggage.hpp"
#include "baggage_norm.hpp"

namespace baggage
{

class IFeePolicy
{
public:
    virtual ~IFeePolicy() = default;

    [[nodiscard]] virtual double FeeFor(const BaseBaggage& piece, const BaggageNorm& norm) const = 0;
    [[nodiscard]] virtual std::string Name() const = 0;
};

} // namespace baggage

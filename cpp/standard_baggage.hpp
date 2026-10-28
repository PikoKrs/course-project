#pragma once

#include "base_baggage.hpp"

#include <string>

namespace baggage
{

class StandardBaggage : public BaseBaggage
{
public:
    static constexpr double kFeePerKg = 500.0;

    StandardBaggage();
    StandardBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg, double lengthCm,
                    double widthCm, double heightCm);
    StandardBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg, double dimsSumCm);
    ~StandardBaggage() override;

    [[nodiscard]] std::unique_ptr<BaseBaggage> Clone() const override;
    [[nodiscard]] double ExcessFee(const BaggageNorm& norm) const override;
    [[nodiscard]] std::string Describe() const override;
};

} // namespace baggage

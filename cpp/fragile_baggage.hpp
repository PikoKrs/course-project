#pragma once

#include "base_baggage.hpp"

#include <string>

namespace baggage
{

class FragileBaggage final : public BaseBaggage
{
public:
    FragileBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg, double lengthCm,
                   double widthCm, double heightCm);
    FragileBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg, double dimsSumCm);
    ~FragileBaggage() override;

    [[nodiscard]] std::unique_ptr<BaseBaggage> Clone() const override;
    [[nodiscard]] double ExcessFee(const BaggageNorm& norm) const override;
    [[nodiscard]] std::string Describe() const override;

    void MarkDamaged();

private:
    bool m_damageMarked;
};

} // namespace baggage

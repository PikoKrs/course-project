#pragma once

#include "base_baggage.hpp"
#include "equipment_spec.hpp"
#include "is_scannable.hpp"

#include <memory>
#include <string>

namespace baggage
{

class OversizeBaggage : public BaseBaggage, public IScannable
{
public:
    static constexpr double kFixedFee = 2500.0;

    OversizeBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg, double lengthCm,
                    double widthCm, double heightCm, const std::string& itemName, double itemLengthCm);
    OversizeBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg, double dimsSumCm,
                    const std::string& itemName, double itemLengthCm);
    OversizeBaggage(const OversizeBaggage& other);
    OversizeBaggage& operator=(const OversizeBaggage& other);
    OversizeBaggage(OversizeBaggage&& other) noexcept;
    OversizeBaggage& operator=(OversizeBaggage&& other) noexcept;
    ~OversizeBaggage() noexcept override;

    [[nodiscard]] std::unique_ptr<BaseBaggage> Clone() const override;
    [[nodiscard]] double ExcessFee(const BaggageNorm& norm) const override;
    [[nodiscard]] std::string Describe() const override;

    [[nodiscard]] std::string ScanBarcode() const noexcept override;

    void RenameItem(const std::string& itemName, double itemLengthCm);

private:
    std::shared_ptr<EquipmentSpec> m_spec;
};

} // namespace baggage

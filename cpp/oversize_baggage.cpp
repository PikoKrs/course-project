#include "oversize_baggage.hpp"

#include <iostream>

namespace baggage
{

OversizeBaggage::OversizeBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg,
                                 double lengthCm, double widthCm, double heightCm, const std::string& itemName,
                                 double itemLengthCm)
    : BaseBaggage{ tagNumber, issueDate, weightKg, lengthCm, widthCm, heightCm }
    , m_spec{ std::make_shared<EquipmentSpec>(itemName, itemLengthCm) }
{
    std::cout << "[Негабарит] Оформлено негабаритное место багажа: " << m_spec->ToString() << ", фиксированный сбор "
              << kFixedFee << " руб.\n";
}

OversizeBaggage::~OversizeBaggage()
{
    std::cout << "[Негабарит] Негабаритное место багажа " << GetTagNumber() << " уничтожено (деструктор наследника)\n";
}

double OversizeBaggage::ExcessFee(const BaggageNorm& norm) const
{
    (void)norm;
    return kFixedFee;
}

std::string OversizeBaggage::Describe() const
{
    return "Негабаритное место багажа " + GetTagNumber() + ": предмет " + m_spec->ToString() + ", вес "
           + std::to_string(m_weightKg) + " кг, габариты " + std::to_string(m_dimsSumCm) + " см, фиксированный сбор";
}

void OversizeBaggage::RenameItem(const std::string& itemName, double itemLengthCm)
{
    m_spec->ChangeItem(itemName, itemLengthCm);
}

} // namespace baggage

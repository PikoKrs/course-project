#include "equipment_spec.hpp"

namespace baggage
{

EquipmentSpec::EquipmentSpec(const std::string& itemName, double itemLengthCm)
    : m_itemName{ itemName }
    , m_itemLengthCm{ itemLengthCm }
{
}

void EquipmentSpec::ChangeItem(const std::string& itemName, double itemLengthCm) noexcept
{
    m_itemName     = itemName;
    m_itemLengthCm = itemLengthCm;
}

std::string EquipmentSpec::ToString() const
{
    return m_itemName + " (" + std::to_string(m_itemLengthCm) + " см)";
}

} // namespace baggage

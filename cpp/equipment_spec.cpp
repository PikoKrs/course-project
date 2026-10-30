#include "equipment_spec.hpp"

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
    return m_itemName + " (" + FormatNumber(m_itemLengthCm) + " см)";
}

} // namespace baggage

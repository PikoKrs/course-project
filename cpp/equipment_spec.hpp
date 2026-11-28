#pragma once

#include <string>

namespace baggage
{

class EquipmentSpec
{
public:
    EquipmentSpec(const std::string& itemName, double itemLengthCm);

    [[nodiscard]] const std::string& GetItemName() const noexcept
    {
        return m_itemName;
    }

    [[nodiscard]] double GetItemLengthCm() const noexcept
    {
        return m_itemLengthCm;
    }

    void ChangeItem(const std::string& itemName, double itemLengthCm) noexcept;

    [[nodiscard]] std::string ToString() const;

private:
    std::string m_itemName;
    double m_itemLengthCm;
};

} // namespace baggage

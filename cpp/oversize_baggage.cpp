#include "oversize_baggage.hpp"

#include <iostream>
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

OversizeBaggage::OversizeBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg,
                                 double lengthCm, double widthCm, double heightCm, const std::string& itemName,
                                 double itemLengthCm)
    : BaseBaggage{ tagNumber, issueDate, weightKg, lengthCm, widthCm, heightCm }
    , m_spec{ std::make_shared<EquipmentSpec>(itemName, itemLengthCm) }
{
    std::cout << "[Негабарит] Оформлено негабаритное место багажа: " << m_spec->ToString() << ", фиксированный сбор "
              << kFixedFee << " руб.\n";
}

OversizeBaggage::OversizeBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg,
                                 double dimsSumCm, const std::string& itemName, double itemLengthCm)
    : BaseBaggage{ tagNumber, issueDate, weightKg, dimsSumCm }
    , m_spec{ std::make_shared<EquipmentSpec>(itemName, itemLengthCm) }
{
    std::cout << "[Негабарит] Оформлено негабаритное место багажа: " << m_spec->ToString() << ", фиксированный сбор "
              << kFixedFee << " руб.\n";
}

OversizeBaggage::OversizeBaggage(const OversizeBaggage& other)
    : BaseBaggage{ BaggageTag::GenerateNumber(), other.TagIssueDate(), other.m_weightKg, other.m_dimsSumCm }
    , m_spec{ std::make_shared<EquipmentSpec>(other.m_spec->GetItemName(), other.m_spec->GetItemLengthCm()) }
{
    m_status = other.m_status;
    std::cout << "[Негабарит] Глубокое копирование: новая бирка " << GetTagNumber()
              << ", собственный экземпляр спецификации\n";
}

OversizeBaggage& OversizeBaggage::operator=(const OversizeBaggage& other)
{
    if (this == &other)
    {
        std::cout << "[Негабарит] Самоприсваивание отклонено (this == &other)\n";
        return *this;
    }

    m_tag       = std::make_shared<BaggageTag>(BaggageTag::GenerateNumber(), other.TagIssueDate());
    m_weightKg  = other.m_weightKg;
    m_dimsSumCm = other.m_dimsSumCm;
    m_status    = other.m_status;
    m_spec      = std::make_shared<EquipmentSpec>(other.m_spec->GetItemName(), other.m_spec->GetItemLengthCm());

    std::cout << "[Негабарит] Присваивание: новая бирка " << GetTagNumber() << ", собственная спецификация\n";
    return *this;
}

OversizeBaggage::OversizeBaggage(OversizeBaggage&& other) noexcept
    : BaseBaggage{ std::move(other) }
    , m_spec{ std::move(other.m_spec) }
{
    std::cout << "[Негабарит] Перемещение: бирка и спецификация переехали без копирования\n";
}

OversizeBaggage& OversizeBaggage::operator=(OversizeBaggage&& other) noexcept
{
    if (this == &other)
    {
        std::cout << "[Негабарит] Самоприсваивание отклонено (this == &other)\n";
        return *this;
    }

    BaseBaggage::operator=(std::move(other));
    m_spec = std::move(other.m_spec);

    std::cout << "[Негабарит] Перемещающее присваивание выполнено\n";
    return *this;
}

OversizeBaggage::~OversizeBaggage() noexcept
{
    if (m_tag)
    {
        std::cout << "[Негабарит] Негабаритное место багажа " << GetTagNumber()
                  << " уничтожено (деструктор наследника)\n";
    }
    else
    {
        std::cout << "[Негабарит] Освобождена перемещённая часть негабаритного места (деструктор наследника)\n";
    }
}

std::unique_ptr<BaseBaggage> OversizeBaggage::Clone() const
{
    auto clone = std::make_unique<OversizeBaggage>(BaggageTag::GenerateNumber(), TagIssueDate(), m_weightKg,
                                                   m_dimsSumCm, m_spec->GetItemName(), m_spec->GetItemLengthCm());
    clone->m_status = m_status;
    std::cout << "[Негабарит] Глубокая копия: у места собственная бирка " << clone->GetTagNumber()
              << " и собственная спецификация\n";
    return clone;
}

double OversizeBaggage::ExcessFee(const BaggageNorm& norm) const
{
    (void)norm;
    return kFixedFee;
}

std::string OversizeBaggage::Describe() const
{
    return "Негабаритное место багажа " + GetTagNumber() + ": предмет " + m_spec->ToString() + ", вес "
           + FormatNumber(m_weightKg) + " кг, габариты " + FormatNumber(m_dimsSumCm) + " см, фиксированный сбор";
}

void OversizeBaggage::RenameItem(const std::string& itemName, double itemLengthCm)
{
    m_spec->ChangeItem(itemName, itemLengthCm);
}

std::string OversizeBaggage::ScanBarcode() const noexcept
{
    std::cout << "[Сканер] Считан штрих-код бирки негабарита: " << GetTagNumber() << " (предмет: " << m_spec->ToString()
              << ")\n";
    return GetTagNumber();
}

} // namespace baggage

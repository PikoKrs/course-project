#include "fragile_baggage.hpp"

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

FragileBaggage::FragileBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg,
                               double lengthCm, double widthCm, double heightCm)
    : BaseBaggage{ tagNumber, issueDate, weightKg, lengthCm, widthCm, heightCm }
    , m_damageMarked{ false }
{
    std::cout << "[Хрупкое] Оформлено хрупкое место багажа (акты о повреждении фиксируются по сценарию 5)\n";
}

FragileBaggage::FragileBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg,
                               double dimsSumCm)
    : BaseBaggage{ tagNumber, issueDate, weightKg, dimsSumCm }
    , m_damageMarked{ false }
{
    std::cout << "[Хрупкое] Оформлено хрупкое место багажа (акты о повреждении фиксируются по сценарию 5)\n";
}

FragileBaggage::~FragileBaggage()
{
    std::cout << "[Хрупкое] Хрупкое место багажа " << GetTagNumber() << " уничтожено (деструктор наследника)\n";
}

std::unique_ptr<BaseBaggage> FragileBaggage::Clone() const
{
    auto clone = std::make_unique<FragileBaggage>(BaggageTag::GenerateNumber(), TagIssueDate(), m_weightKg,
                                                  m_dimsSumCm);
    clone->m_status = m_status;
    if (m_damageMarked)
    {
        clone->MarkDamaged();
    }
    std::cout << "[Хрупкое] Глубокая копия: у места собственная бирка " << clone->GetTagNumber() << "\n";
    return clone;
}

double FragileBaggage::ExcessFee(const BaggageNorm& norm) const
{
    (void)norm;
    return 0.0;
}

std::string FragileBaggage::Describe() const
{
    if (m_damageMarked)
    {
        return "Хрупкое место багажа " + GetTagNumber() + ": вес " + FormatNumber(m_weightKg)
               + " кг, статус: выдано с пометкой \"повреждено\" (сценарий 5)";
    }

    return "Хрупкое место багажа " + GetTagNumber() + ": вес " + FormatNumber(m_weightKg)
           + " кг, повреждений нет";
}

void FragileBaggage::MarkDamaged()
{
    m_damageMarked = true;
    std::cout << "[Хрупкое] К месту багажа " << GetTagNumber() << " добавлена пометка \"повреждено\"\n";
}

} // namespace baggage

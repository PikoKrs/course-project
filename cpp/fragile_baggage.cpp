#include "fragile_baggage.hpp"

#include <iostream>

namespace baggage
{

FragileBaggage::FragileBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg,
                               double lengthCm, double widthCm, double heightCm)
    : BaseBaggage{ tagNumber, issueDate, weightKg, lengthCm, widthCm, heightCm }
    , m_damageMarked{ false }
{
    std::cout << "[Хрупкое] Оформлено хрупкое место багажа (акты о повреждении фиксируются по сценарию 5)\n";
}

FragileBaggage::~FragileBaggage()
{
    std::cout << "[Хрупкое] Хрупкое место багажа " << GetTagNumber() << " уничтожено (деструктор наследника)\n";
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
        return "Хрупкое место багажа " + GetTagNumber() + ": вес " + std::to_string(m_weightKg)
               + " кг, статус: выдано с пометкой \"повреждено\" (сценарий 5)";
    }

    return "Хрупкое место багажа " + GetTagNumber() + ": вес " + std::to_string(m_weightKg)
           + " кг, повреждений нет";
}

void FragileBaggage::MarkDamaged()
{
    m_damageMarked = true;
    std::cout << "[Хрупкое] К месту багажа " << GetTagNumber() << " добавлена пометка \"повреждено\"\n";
}

} // namespace baggage

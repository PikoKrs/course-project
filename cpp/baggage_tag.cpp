#include "baggage_tag.hpp"

#include <iostream>

namespace baggage
{

BaggageTag::BaggageTag()
    : m_number{ "000000" }
    , m_issueDate{ "-" }
{
    std::cout << "[Бирка] Выдана временная бирка 000000 (номер не присвоен)\n";
}

BaggageTag::BaggageTag(const std::string& number, const std::string& issueDate)
    : m_number{ number }
    , m_issueDate{ issueDate }
{
    if (m_number.empty())
    {
        std::cout << "[Бирка] Отказ: номер бирки не может быть пустым (правило 2)\n";
        m_number  = "000000";
        m_issueDate = "-";
        std::cout << "[Бирка] Выдана временная бирка 000000 (номер не присвоен)\n";
    }
    else
    {
        std::cout << "[Бирка] Оформлена бирка " << m_number << " (дата " << m_issueDate << ")\n";
    }
}

BaggageTag::~BaggageTag()
{
    std::cout << "[Бирка] Бирка " << m_number << " уничтожена\n";
}

bool BaggageTag::VerifyAgainstStub(const std::string& stubNumber) const
{
    if (m_number == stubNumber)
    {
        std::cout << "[Бирка] Номер " << m_number << " совпадает с отрывной частью: место выдано\n";
        return true;
    }

    std::cout << "[Бирка] Отказ: номер " << m_number << " не совпадает с отрывной частью " << stubNumber
              << " (правило 10)\n";
    return false;
}

void BaggageTag::PrintInfo() const
{
    std::cout << "[Бирка] Номер: " << m_number << ", дата оформления: " << m_issueDate << "\n";
}

} // namespace baggage

#include "baggage_tag.hpp"

#include <iostream>
#include <set>

#include "baggage_rule_error.hpp"

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
        throw DuplicateTagError{ "номер бирки не может быть пустым" };
    }

    static std::set<std::string> s_issuedNumbers;
    if (!s_issuedNumbers.insert(m_number).second)
    {
        throw DuplicateTagError{ "номер бирки " + m_number + " уже выдан, номера бирок не повторяются" };
    }

    std::cout << "[Бирка] Оформлена бирка " << m_number << " (дата " << m_issueDate << ")\n";
}

BaggageTag::~BaggageTag()
{
    std::cout << "[Бирка] Бирка " << m_number << " уничтожена\n";
}

std::string BaggageTag::GenerateNumber()
{
    static int s_nextNumber = 240110;
    const std::string number = "БАГ-" + std::to_string(s_nextNumber);
    ++s_nextNumber;
    return number;
}

void BaggageTag::ChangeIssueDate(const std::string& issueDate)
{
    m_issueDate = issueDate;
    std::cout << "[Бирка] Бирка " << m_number << " переоформлена на дату " << m_issueDate << "\n";
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

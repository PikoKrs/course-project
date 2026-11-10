#include "base_baggage.hpp"

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

const char* StatusNameImpl(baggage::BaggageStatus status) noexcept
{
    switch (status)
    {
        case baggage::BaggageStatus::eNone:
            return "не оформлено";
        case baggage::BaggageStatus::eAccepted:
            return "принято";
        case baggage::BaggageStatus::eScreening:
            return "на досмотре";
        case baggage::BaggageStatus::eSorted:
            return "отсортировано";
        case baggage::BaggageStatus::eLoaded:
            return "загружено";
        case baggage::BaggageStatus::eInTransit:
            return "в пути";
        case baggage::BaggageStatus::eIssued:
            return "выдано";
        case baggage::BaggageStatus::eDelayed:
            return "задержано";
        case baggage::BaggageStatus::eLost:
            return "утеряно";
        case baggage::BaggageStatus::eRemoved:
            return "снято с рейса";
    }
    return "?";
}

} // namespace

namespace baggage
{

BaseBaggage::BaseBaggage()
    : m_tag{ std::make_shared<BaggageTag>() }
    , m_weightKg{ 0.0 }
    , m_dimsSumCm{ 0.0 }
    , m_status{ BaggageStatus::eNone }
{
    std::cout << "[Базовый] Создано неоформленное место багажа (вес 0 кг)\n";
}

BaseBaggage::BaseBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg, double lengthCm,
                         double widthCm, double heightCm)
    : m_tag{ std::make_shared<BaggageTag>(tagNumber, issueDate) }
    , m_weightKg{ weightKg }
    , m_dimsSumCm{ lengthCm + widthCm + heightCm }
    , m_status{ BaggageStatus::eNone }
{
    std::cout << "[Базовый] Сформировано место багажа: вес " << m_weightKg << " кг, габариты " << m_dimsSumCm
              << " см\n";
}

BaseBaggage::BaseBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg,
                         double dimsSumCm)
    : m_tag{ std::make_shared<BaggageTag>(tagNumber, issueDate) }
    , m_weightKg{ weightKg }
    , m_dimsSumCm{ dimsSumCm }
    , m_status{ BaggageStatus::eNone }
{
    std::cout << "[Базовый] Сформировано место багажа: вес " << m_weightKg << " кг, габариты " << m_dimsSumCm
              << " см\n";
}

BaseBaggage::~BaseBaggage() noexcept
{
    if (m_tag)
    {
        std::cout << "[Базовый] Место багажа " << m_tag->GetNumber() << " уничтожено (виртуальный деструктор базы)\n";
    }
    else
    {
        std::cout << "[Базовый] Освобождена перемещённая часть места багажа\n";
    }
}

void BaseBaggage::AcceptAtCheckIn(const BaggageNorm& norm)
{
    if (m_weightKg > norm.GetAbsoluteMaxWeightKg())
    {
        throw OverweightError{ m_weightKg };
    }

    if (m_status != BaggageStatus::eNone)
    {
        throw BaggageRuleError{ "место багажа " + GetTagNumber() + " уже в обработке (статус: " + StatusName(m_status)
                                + "), повторный приём недопустим" };
    }

    m_status = BaggageStatus::eAccepted;
    const double fee = ExcessFee(norm);
    if (fee > 0.0)
    {
        std::cout << "[Место] Место багажа " << GetTagNumber() << " принято со сверхнормативом, сбор за перевес: "
                  << fee << " руб.\n";
    }
    else
    {
        std::cout << "[Место] Место багажа " << GetTagNumber() << " принято, укладывается в норму тарифа\n";
    }
}

void BaseBaggage::MarkStatus(BaggageStatus newStatus)
{
    const bool newIsProblem = newStatus == BaggageStatus::eDelayed || newStatus == BaggageStatus::eLost
                              || newStatus == BaggageStatus::eRemoved;
    const bool currentIsProblem = m_status == BaggageStatus::eDelayed || m_status == BaggageStatus::eLost
                                  || m_status == BaggageStatus::eRemoved;

    if (newStatus == m_status)
    {
        throw BaggageRuleError{ "правило 12: статус места багажа " + GetTagNumber() + " уже "
                                + StatusName(m_status) };
    }

    if (m_status == BaggageStatus::eNone && newStatus != BaggageStatus::eAccepted)
    {
        throw BaggageRuleError{ "правило 12: место багажа " + GetTagNumber()
                                + " ещё не принято, первый статус должен быть \"принято\"" };
    }

    if (!newIsProblem && !currentIsProblem && newStatus != BaggageStatus::eNone
        && static_cast<int>(newStatus) < static_cast<int>(m_status))
    {
        throw BaggageRuleError{ "правило 12: статус места багажа не меняется в обратную сторону: "
                                + std::string{ StatusName(m_status) } + " -> " + StatusName(newStatus) };
    }

    m_status = newStatus;
    std::cout << "[Место] Бирка " << GetTagNumber() << ": новый статус: " << StatusName(newStatus) << "\n";
}

bool BaseBaggage::VerifyTagAgainstStub(const std::string& stubNumber) const
{
    return m_tag->VerifyAgainstStub(stubNumber);
}

void BaseBaggage::ReissueDate(const std::string& issueDate)
{
    m_tag->ChangeIssueDate(issueDate);
}

const std::string& BaseBaggage::GetTagNumber() const noexcept
{
    static const std::string kMovedFrom{ "перемещено" };
    return m_tag ? m_tag->GetNumber() : kMovedFrom;
}

void BaseBaggage::SetDims(double lengthCm, double widthCm, double heightCm) noexcept
{
    m_dimsSumCm = lengthCm + widthCm + heightCm;
}

std::string BaseBaggage::Describe() const
{
    return "Место багажа " + GetTagNumber() + ": вес " + FormatNumber(m_weightKg) + " кг, габариты "
           + FormatNumber(m_dimsSumCm) + " см, статус: " + StatusName(m_status);
}

void BaseBaggage::PrintState() const
{
    std::cout << Describe() << "\n";
}

const std::string& BaseBaggage::TagIssueDate() const noexcept
{
    return m_tag->GetIssueDate();
}

const char* BaseBaggage::StatusName(BaggageStatus status) noexcept
{
    return StatusNameImpl(status);
}

} // namespace baggage

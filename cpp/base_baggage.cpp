#include "base_baggage.hpp"

#include <iostream>

namespace
{

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
    : m_tag{}
    , m_weightKg{ 0.0 }
    , m_dimsSumCm{ 0.0 }
    , m_status{ BaggageStatus::eNone }
{
    std::cout << "[Базовый] Создано неоформленное место багажа (вес 0 кг)\n";
}

BaseBaggage::BaseBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg, double lengthCm,
                         double widthCm, double heightCm)
    : m_tag{ tagNumber, issueDate }
    , m_weightKg{ weightKg }
    , m_dimsSumCm{ lengthCm + widthCm + heightCm }
    , m_status{ BaggageStatus::eNone }
{
    std::cout << "[Базовый] Сформировано место багажа: вес " << m_weightKg << " кг, габариты " << m_dimsSumCm
              << " см\n";
}

BaseBaggage::BaseBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg,
                         double dimsSumCm)
    : m_tag{ tagNumber, issueDate }
    , m_weightKg{ weightKg }
    , m_dimsSumCm{ dimsSumCm }
    , m_status{ BaggageStatus::eNone }
{
    std::cout << "[Базовый] Сформировано место багажа: вес " << m_weightKg << " кг, габариты " << m_dimsSumCm
              << " см\n";
}

BaseBaggage::~BaseBaggage()
{
    std::cout << "[Базовый] Место багажа " << m_tag.GetNumber() << " уничтожено (деструктор базы)\n";
}

bool BaseBaggage::AcceptAtCheckIn(const BaggageNorm& norm)
{
    if (m_weightKg > norm.GetAbsoluteMaxWeightKg())
    {
        std::cout << "[Место] Отказ: вес места багажа " << m_weightKg << " кг превышает предельные "
                  << norm.GetAbsoluteMaxWeightKg() << " кг (правило 4), место не принимается\n";
        return false;
    }

    if (m_status != BaggageStatus::eNone)
    {
        std::cout << "[Место] Отказ: место багажа " << m_tag.GetNumber() << " уже в обработке (статус: "
                  << StatusName(m_status) << ")\n";
        return false;
    }

    m_status = BaggageStatus::eAccepted;
    const double fee = ExcessFee(norm);
    if (fee > 0.0)
    {
        std::cout << "[Место] Место багажа " << m_tag.GetNumber() << " принято со сверхнормативом, сбор за перевес: "
                  << fee << " руб.\n";
    }
    else
    {
        std::cout << "[Место] Место багажа " << m_tag.GetNumber() << " принято, укладывается в норму тарифа\n";
    }
    return true;
}

bool BaseBaggage::MarkStatus(BaggageStatus newStatus)
{
    const bool newIsProblem = newStatus == BaggageStatus::eDelayed || newStatus == BaggageStatus::eLost
                              || newStatus == BaggageStatus::eRemoved;
    const bool currentIsProblem = m_status == BaggageStatus::eDelayed || m_status == BaggageStatus::eLost
                                  || m_status == BaggageStatus::eRemoved;

    if (newStatus == m_status)
    {
        std::cout << "[Место] Бирка " << m_tag.GetNumber() << ": статус уже " << StatusName(m_status) << "\n";
        return false;
    }

    if (m_status == BaggageStatus::eNone && newStatus != BaggageStatus::eAccepted)
    {
        std::cout << "[Место] Отказ: место багажа " << m_tag.GetNumber()
                  << " ещё не принято, первый статус должен быть \"принято\" (правило 12)\n";
        return false;
    }

    if (!newIsProblem && !currentIsProblem && newStatus != BaggageStatus::eNone
        && static_cast<int>(newStatus) < static_cast<int>(m_status))
    {
        std::cout << "[Место] Отказ: статус места багажа не меняется в обратную сторону: " << StatusName(m_status)
                  << " -> " << StatusName(newStatus) << " (правило 12)\n";
        return false;
    }

    m_status = newStatus;
    std::cout << "[Место] Бирка " << m_tag.GetNumber() << ": новый статус: " << StatusName(newStatus) << "\n";
    return true;
}

bool BaseBaggage::VerifyTagAgainstStub(const std::string& stubNumber) const
{
    return m_tag.VerifyAgainstStub(stubNumber);
}

void BaseBaggage::SetDims(double lengthCm, double widthCm, double heightCm) noexcept
{
    m_dimsSumCm = lengthCm + widthCm + heightCm;
}

std::string BaseBaggage::Describe() const
{
    return "Место багажа " + m_tag.GetNumber() + ": вес " + std::to_string(m_weightKg) + " кг, габариты "
           + std::to_string(m_dimsSumCm) + " см, статус: " + StatusName(m_status);
}

void BaseBaggage::PrintState() const
{
    std::cout << Describe() << "\n";
}

const char* BaseBaggage::StatusName(BaggageStatus status) noexcept
{
    return StatusNameImpl(status);
}

} // namespace baggage

#include "baggage_piece.hpp"

#include <iostream>

namespace
{

const char* StatusName(baggage::BaggageStatus status)
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

BaggagePiece::BaggagePiece()
    : m_tag{}
    , m_weightKg{ 0.0 }
    , m_dimsSumCm{ 0.0 }
    , m_status{ BaggageStatus::eNone }
{
    std::cout << "[Место] Создано неоформленное место багажа (вес 0 кг)\n";
}

BaggagePiece::BaggagePiece(const std::string& tagNumber, const std::string& issueDate, double weightKg,
                           double lengthCm, double widthCm, double heightCm)
    : m_tag{ tagNumber, issueDate }
    , m_weightKg{ weightKg }
    , m_dimsSumCm{ lengthCm + widthCm + heightCm }
    , m_status{ BaggageStatus::eNone }
{
    std::cout << "[Место] Создано место багажа: вес " << m_weightKg << " кг, габариты " << m_dimsSumCm << " см\n";
}

BaggagePiece::~BaggagePiece()
{
    std::cout << "[Место] Место багажа " << m_tag.GetNumber() << " уничтожено\n";
}

bool BaggagePiece::AcceptAtCheckIn()
{
    if (m_weightKg > 32.0)
    {
        std::cout << "[Место] Отказ: вес места багажа " << m_weightKg << " кг превышает предельные 32 кг (правило 4),"
                     " место не принимается\n";
        return false;
    }

    if (m_status != BaggageStatus::eNone)
    {
        std::cout << "[Место] Отказ: место багажа " << m_tag.GetNumber() << " уже в обработке (статус: "
                  << StatusName(m_status) << ")\n";
        return false;
    }

    m_status = BaggageStatus::eAccepted;
    std::cout << "[Место] Место багажа " << m_tag.GetNumber() << " принято на рейс (вес " << m_weightKg
              << " кг), статус: принято\n";
    return true;
}

bool BaggagePiece::MarkStatus(BaggageStatus newStatus)
{
    const bool newIsProblem =
        newStatus == BaggageStatus::eDelayed || newStatus == BaggageStatus::eLost
        || newStatus == BaggageStatus::eRemoved;
    const bool currentIsProblem =
        m_status == BaggageStatus::eDelayed || m_status == BaggageStatus::eLost || m_status == BaggageStatus::eRemoved;

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

void BaggagePiece::SetDims(double lengthCm, double widthCm, double heightCm) noexcept
{
    m_dimsSumCm = lengthCm + widthCm + heightCm;
}

void BaggagePiece::PrintState() const
{
    std::cout << "[Место] Бирка " << m_tag.GetNumber() << ": вес " << m_weightKg << " кг, сумма габаритов "
              << m_dimsSumCm << " см, статус: " << StatusName(m_status) << "\n";
}

} // namespace baggage

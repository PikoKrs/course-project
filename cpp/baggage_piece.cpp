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
    m_status = BaggageStatus::eAccepted;
    std::cout << "[Место] Место багажа " << m_tag.GetNumber() << " принято на рейс (вес " << m_weightKg
              << " кг), статус: принято\n";
    return true;
}

bool BaggagePiece::MarkStatus(BaggageStatus newStatus)
{
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

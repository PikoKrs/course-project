#include "flight.hpp"

#include <iostream>

namespace baggage
{

Flight::Flight(const std::string& number, const std::string& fromAirport, const std::string& toAirport,
               int capacityPlaces, double maxTotalWeightKg)
    : m_number{ number }
    , m_fromAirport{ fromAirport }
    , m_toAirport{ toAirport }
    , m_capacityPlaces{ capacityPlaces }
    , m_maxTotalWeightKg{ maxTotalWeightKg }
    , m_checkInClosed{ false }
    , m_manifest{}
{
    std::cout << "[Рейс] Открыт рейс " << m_number << " (" << m_fromAirport << " -> " << m_toAirport
              << "): вместимость " << m_capacityPlaces << " мест, лимит веса " << m_maxTotalWeightKg << " кг\n";
}

Flight::~Flight()
{
    std::cout << "[Рейс] Рейс " << m_number << " закрыт; мест в манифесте: " << m_manifest.size()
              << " (места багажа рейсу не принадлежат)\n";
}

bool Flight::AddBaggage(const std::shared_ptr<BaggagePiece>& piece)
{
    if (m_checkInClosed)
    {
        std::cout << "[Рейс] Отказ: регистрация на рейс " << m_number << " закрыта, приём багажа прекращён (правило 9)\n";
        return false;
    }

    if (LoadedCount() >= m_capacityPlaces)
    {
        std::cout << "[Рейс] Отказ: вместимость отсека рейса " << m_number << " исчерпана (" << m_capacityPlaces
                  << " мест), назначение места багажа " << piece->GetTagNumber() << " невозможно (правило 7)\n";
        return false;
    }

    const double newTotalWeight = LoadedWeightKg() + piece->GetWeightKg();
    if (newTotalWeight > m_maxTotalWeightKg)
    {
        std::cout << "[Рейс] Отказ: суммарный вес багажа рейса " << m_number << " (" << newTotalWeight
                  << " кг) превысил бы лимит " << m_maxTotalWeightKg << " кг (правило 7)\n";
        return false;
    }

    m_manifest.emplace_back(piece);
    std::cout << "[Рейс] Место багажа " << piece->GetTagNumber() << " назначено на рейс " << m_number << " (мест: "
              << m_manifest.size() << " из " << m_capacityPlaces << ", вес: " << newTotalWeight << " из "
              << m_maxTotalWeightKg << " кг)\n";
    return true;
}

bool Flight::RemoveBaggage(const std::string& tagNumber)
{
    for (std::size_t i = 0; i < m_manifest.size(); ++i)
    {
        const std::shared_ptr<BaggagePiece> piece = m_manifest[i].lock();
        if (piece && piece->GetTagNumber() == tagNumber)
        {
            std::cout << "[Рейс] Место багажа " << tagNumber << " снято с рейса " << m_number << "\n";
            m_manifest.erase(m_manifest.begin() + static_cast<std::ptrdiff_t>(i));
            return true;
        }
    }

    std::cout << "[Рейс] Место багажа " << tagNumber << " не найдено в манифесте рейса " << m_number << "\n";
    return false;
}

void Flight::CloseCheckIn()
{
    if (m_checkInClosed)
    {
        std::cout << "[Рейс] Регистрация на рейс " << m_number << " уже закрыта\n";
        return;
    }

    m_checkInClosed = true;
    std::cout << "[Рейс] Регистрация на рейс " << m_number << " закрыта, приём багажа прекращён (правило 9)\n";
}

void Flight::PrintManifest() const
{
    std::cout << "[Рейс] Манифест рейса " << m_number << " (" << m_fromAirport << " -> " << m_toAirport << "): мест "
              << m_manifest.size() << " из " << m_capacityPlaces << "\n";

    for (const std::weak_ptr<BaggagePiece>& entry : m_manifest)
    {
        const std::shared_ptr<BaggagePiece> piece = entry.lock();
        if (!piece)
        {
            std::cout << "  [Место] место багажа недоступно: владелец его уничтожил (weak_ptr lock() пуст)\n";
            continue;
        }

        std::cout << "  ";
        piece->PrintState();
    }
}

double Flight::LoadedWeightKg() const noexcept
{
    double total = 0.0;
    for (const std::weak_ptr<BaggagePiece>& entry : m_manifest)
    {
        if (const std::shared_ptr<BaggagePiece> piece = entry.lock())
        {
            total += piece->GetWeightKg();
        }
    }
    return total;
}

} // namespace baggage

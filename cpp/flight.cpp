#include "flight.hpp"

#include <iostream>

#include "baggage_rule_error.hpp"

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

void Flight::AddBaggage(const std::shared_ptr<BaseBaggage>& piece)
{
    if (m_checkInClosed)
    {
        throw CheckinClosedError{ m_number };
    }

    if (LoadedCount() >= m_capacityPlaces)
    {
        throw FlightCapacityError{ m_number, "все " + std::to_string(m_capacityPlaces) + " мест отсека заняты" };
    }

    const double newTotalWeight = LoadedWeightKg() + piece->GetWeightKg();
    if (newTotalWeight > m_maxTotalWeightKg)
    {
        throw FlightCapacityError{ m_number, "суммарный вес багажа " + std::to_string(newTotalWeight)
                                                  + " кг превысил бы лимит " + std::to_string(m_maxTotalWeightKg)
                                                  + " кг" };
    }

    m_manifest.emplace_back(piece);
    std::cout << "[Рейс] Место багажа " << piece->GetTagNumber() << " назначено на рейс " << m_number << " (мест: "
              << m_manifest.size() << " из " << m_capacityPlaces << ", вес: " << newTotalWeight << " из "
              << m_maxTotalWeightKg << " кг)\n";
}

bool Flight::RemoveBaggage(const std::string& tagNumber)
{
    for (std::size_t i = 0; i < m_manifest.size(); ++i)
    {
        const std::shared_ptr<BaseBaggage> piece = m_manifest[i].lock();
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

void Flight::ParseNumber(const std::string& raw)
{
    const std::size_t dashPos = raw.find('-');
    if (dashPos == std::string::npos)
    {
        std::cout << "[Рейс] Строка \"" << raw << "\" не содержит разделитель \"-\": разбор невозможен\n";
        return;
    }

    const std::string airline = raw.substr(0, dashPos);
    const std::string number  = raw.substr(dashPos + 1);
    std::cout << "[Рейс] Разбор строки \"" << raw << "\": авиакомпания \"" << airline << "\", номер \"" << number
              << "\"\n";

    if (airline == "S7")
    {
        const std::string systemName = "S7-" + number;
        std::cout << "[Рейс] Рейс " << systemName << " найден в системе: авиакомпания S7 подтверждена\n";
    }
    else
    {
        std::cout << "[Рейс] Код \"" << airline << "\" не совпадает с \"S7\": требуется проверка тарифов\n";
    }
}

void Flight::LoadManifest()
{
    std::cout << "[Рейс] Загрузка манифеста рейса " << m_number << " в багажный отсек\n";

    for (const std::weak_ptr<BaseBaggage>& entry : m_manifest)
    {
        const std::shared_ptr<BaseBaggage> piece = entry.lock();
        if (!piece)
        {
            std::cout << "[Рейс] Место багажа недоступно (владелец его уничтожил), пропуск\n";
            continue;
        }

        if (piece->GetStatus() < BaggageStatus::eSorted)
        {
            throw NotClearedError{ piece->GetTagNumber() };
        }

        piece->MarkStatus(BaggageStatus::eLoaded);
        std::cout << "[Рейс] Место багажа " << piece->GetTagNumber() << " загружено в отсек рейса " << m_number << "\n";
    }

    std::cout << "[Рейс] Загрузка манифеста рейса " << m_number << " завершена\n";
}

void Flight::Assign(BaseBaggage& piece, const std::string& ticketRoute) const
{
    if (ticketRoute.find(m_number) == std::string::npos)
    {
        throw NotOnRouteError{ m_number, ticketRoute };
    }

    std::cout << "[Рейс] Место багажа " << piece.GetTagNumber() << " следует по маршруту " << ticketRoute
              << ": рейс " << m_number << " входит в маршрут билета\n";
}

void Flight::PrintManifest() const
{
    std::cout << "[Рейс] Манифест рейса " << m_number << " (" << m_fromAirport << " -> " << m_toAirport << "): мест "
              << m_manifest.size() << " из " << m_capacityPlaces << "\n";

    for (const std::weak_ptr<BaseBaggage>& entry : m_manifest)
    {
        const std::shared_ptr<BaseBaggage> piece = entry.lock();
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
    for (const std::weak_ptr<BaseBaggage>& entry : m_manifest)
    {
        if (const std::shared_ptr<BaseBaggage> piece = entry.lock())
        {
            total += piece->GetWeightKg();
        }
    }
    return total;
}

} // namespace baggage

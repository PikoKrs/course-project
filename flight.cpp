//
// ФАЙЛ flight.cpp
//
#include "flight.hpp"

#include <iostream>

namespace baggage
{

Flight::Flight(std::string_view number, std::string_view destination, std::size_t maxPieces, double holdCapacityKg)
    : m_number{ std::string(number) }
    , m_destination{ std::string(destination) }
    , m_maxPieces{ maxPieces }
    , m_holdCapacityKg{ holdCapacityKg }
{
    std::cout << "[Flight] Открыт рейс " << m_number << " -> " << m_destination << "; отсек до " << m_holdCapacityKg
              << " кг, не более " << m_maxPieces << " мест багажа\n";
}

Flight::~Flight()
{
    // Агрегация: манифест хранит только weak_ptr, вектор указателей очищается сам,
    // сами места багажа остаются жить, пока жив их владелец
    std::cout << "[~Flight] Рейс " << m_number << " завершён; места багажа продолжают существовать\n";
}

double Flight::GetLoadedWeightKg() const
{
    double total = 0.0;
    for (const auto& entry : m_pieces)
    {
        if (std::shared_ptr<BaggagePiece> piece = entry.lock())
        {
            total += piece->GetWeightKg();
        }
    }
    return total;
}

std::size_t Flight::AliveCount() const
{
    std::size_t count = 0;
    for (const auto& entry : m_pieces)
    {
        if (!entry.expired())
        {
            ++count;
        }
    }
    return count;
}

bool Flight::AddPiece(const std::shared_ptr<BaggagePiece>& piece)
{
    if (!piece)
    {
        std::cout << "[Flight] Отказ: передан пустой указатель на место багажа\n";
        return false;
    }
    if (AliveCount() >= m_maxPieces)
    {
        std::cout << "[Flight] Отказ: на рейс " << m_number << " уже назначено максимум мест (" << m_maxPieces
                  << ")\n";
        return false;
    }
    const double loaded = GetLoadedWeightKg();
    if (loaded + piece->GetWeightKg() > m_holdCapacityKg)
    {
        // Правило 7: суммарный вес мест не может превышать вместимость багажного отсека
        std::cout << "[Flight] Отказ: место " << piece->GetTagNumber() << " (" << piece->GetWeightKg()
                  << " кг) не помещается в отсек: занято " << loaded << " из " << m_holdCapacityKg << " кг\n";
        return false;
    }
    m_pieces.push_back(piece); // shared_ptr -> weak_ptr: рейс не становится владельцем
    std::cout << "[Flight] Место багажа " << piece->GetTagNumber() << " (" << piece->GetOwner()
              << ") назначено на рейс " << m_number << "\n";
    return true;
}

bool Flight::RemovePiece(std::string_view tagNumber)
{
    for (auto it = m_pieces.begin(); it != m_pieces.end(); ++it)
    {
        if (std::shared_ptr<BaggagePiece> piece = it->lock())
        {
            if (piece->GetTagNumber() == tagNumber)
            {
                std::cout << "[Flight] Место багажа " << tagNumber << " снято с рейса " << m_number << "\n";
                m_pieces.erase(it);
                return true;
            }
        }
    }
    std::cout << "[Flight] Отказ: место багажа " << tagNumber << " не значится на рейсе " << m_number << "\n";
    return false;
}

std::size_t Flight::LoadAll()
{
    std::cout << "[Flight] Загрузка багажного отсека рейса " << m_number << "\n";
    std::size_t loaded   = 0;
    std::size_t assigned = 0;
    for (const auto& entry : m_pieces)
    {
        std::shared_ptr<BaggagePiece> piece = entry.lock();
        if (!piece)
        {
            // Место уничтожено, пока оставалось в манифесте: weak_ptr пуст
            std::cout << "[Flight] Запись об уничтоженном месте пропущена (weak_ptr пуст)\n";
            continue;
        }
        ++assigned;
        if (piece->LoadIntoCompartment())
        {
            ++loaded;
        }
        else
        {
            std::cout << "[Flight] Место " << piece->GetTagNumber() << " в отсек не загружено\n";
        }
    }
    std::cout << "[Flight] В отсеке рейса " << loaded << " из " << assigned << " назначенных мест багажа\n";
    return loaded;
}

void Flight::Depart()
{
    std::cout << "[Flight] Рейс " << m_number << " вылетел в " << m_destination << "\n";
    for (const auto& entry : m_pieces)
    {
        if (std::shared_ptr<BaggagePiece> piece = entry.lock())
        {
            piece->MarkInTransit();
        }
    }
}

void Flight::PrintManifest() const
{
    std::cout << "[Flight] Манифест рейса " << m_number << " -> " << m_destination << "; мест: " << AliveCount()
              << ", суммарный вес: " << GetLoadedWeightKg() << " кг\n";
    for (const auto& entry : m_pieces)
    {
        if (std::shared_ptr<BaggagePiece> piece = entry.lock())
        {
            piece->PrintInfo();
        }
        else
        {
            std::cout << "  (запись манифеста указывает на уничтоженное место: weak_ptr пуст)\n";
        }
    }
}

} // namespace baggage

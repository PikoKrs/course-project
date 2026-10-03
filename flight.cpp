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
    , m_pieces{ new BaggagePiece*[maxPieces] }
{
    std::cout << "[Flight] Открыт рейс " << m_number << " -> " << m_destination << "; отсек до " << m_holdCapacityKg
              << " кг, не более " << m_maxPieces << " мест багажа\n";
}

Flight::~Flight()
{
    // Агрегация: уничтожается только массив указателей, сами места багажа остаются жить
    delete[] m_pieces;
    std::cout << "[~Flight] Рейс " << m_number << " завершён; места багажа продолжают существовать\n";
}

double Flight::GetLoadedWeightKg() const
{
    double total = 0.0;
    for (std::size_t i = 0; i < m_count; ++i)
    {
        total += m_pieces[i]->GetWeightKg();
    }
    return total;
}

bool Flight::AddPiece(BaggagePiece* piece)
{
    if (piece == nullptr)
    {
        std::cout << "[Flight] Отказ: передан пустой указатель на место багажа\n";
        return false;
    }
    if (m_count >= m_maxPieces)
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
    m_pieces[m_count] = piece;
    ++m_count;
    std::cout << "[Flight] Место багажа " << piece->GetTagNumber() << " (" << piece->GetOwner()
              << ") назначено на рейс " << m_number << "\n";
    return true;
}

bool Flight::RemovePiece(std::string_view tagNumber)
{
    for (std::size_t i = 0; i < m_count; ++i)
    {
        if (m_pieces[i]->GetTagNumber() == tagNumber)
        {
            std::cout << "[Flight] Место багажа " << tagNumber << " снято с рейса " << m_number << "\n";
            for (std::size_t j = i + 1; j < m_count; ++j)
            {
                m_pieces[j - 1] = m_pieces[j];
            }
            --m_count;
            return true;
        }
    }
    std::cout << "[Flight] Отказ: место багажа " << tagNumber << " не значится на рейсе " << m_number << "\n";
    return false;
}

std::size_t Flight::LoadAll()
{
    std::cout << "[Flight] Загрузка багажного отсека рейса " << m_number << "\n";
    std::size_t loaded = 0;
    for (std::size_t i = 0; i < m_count; ++i)
    {
        if (m_pieces[i]->LoadIntoCompartment())
        {
            ++loaded;
        }
        else
        {
            std::cout << "[Flight] Место " << m_pieces[i]->GetTagNumber() << " в отсек не загружено\n";
        }
    }
    std::cout << "[Flight] В отсеке рейса " << loaded << " из " << m_count << " назначенных мест багажа\n";
    return loaded;
}

void Flight::Depart()
{
    std::cout << "[Flight] Рейс " << m_number << " вылетел в " << m_destination << "\n";
    for (std::size_t i = 0; i < m_count; ++i)
    {
        m_pieces[i]->MarkInTransit();
    }
}

void Flight::PrintManifest() const
{
    std::cout << "[Flight] Манифест рейса " << m_number << " -> " << m_destination << "; мест: " << m_count
              << ", суммарный вес: " << GetLoadedWeightKg() << " кг\n";
    for (std::size_t i = 0; i < m_count; ++i)
    {
        m_pieces[i]->PrintInfo();
    }
}

} // namespace baggage

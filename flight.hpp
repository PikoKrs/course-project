//
// ФАЙЛ flight.hpp
// Рейс: перевозка между двумя аэропортами с фиксированным номером.
// Агрегатор мест багажа: рейс не создаёт места багажа и не уничтожает их,
// он лишь хранит указатели на объекты, созданные вне его (агрегация).
//
#pragma once

#include <cstddef>
#include <string>
#include <string_view>

#include "baggage_piece.hpp"

namespace baggage
{

class Flight
{
public:
    Flight(std::string_view number, std::string_view destination, std::size_t maxPieces, double holdCapacityKg);

    Flight(const Flight&)            = delete;
    Flight(Flight&&)                 = delete;
    Flight& operator=(const Flight&) = delete;
    Flight& operator=(Flight&&)      = delete;
    ~Flight();

    [[nodiscard]] std::string GetNumber() const
    {
        return m_number;
    }

    [[nodiscard]] std::string GetDestination() const
    {
        return m_destination;
    }

    [[nodiscard]] std::size_t GetPieceCount() const
    {
        return m_count;
    }

    [[nodiscard]] double GetLoadedWeightKg() const;

    // Назначить место багажа на рейс; проверяется вместимость отсека (правило 7)
    bool AddPiece(BaggagePiece* piece);

    // Снять место багажа с рейса по номеру бирки (сценарий 3 предметной области)
    bool RemovePiece(std::string_view tagNumber);

    // Загрузка отсека: в рейс попадают только места, прошедшие досмотр (правило 5)
    std::size_t LoadAll();

    // Вылет рейса: загруженные места переходят в статус "в пути"
    void Depart();

    void PrintManifest() const;

private:
    std::string m_number;
    std::string m_destination;
    std::size_t m_maxPieces{};
    double m_holdCapacityKg{};
    BaggagePiece** m_pieces{}; // массив указателей: рейс не владеет местами багажа
    std::size_t m_count{};
};

} // namespace baggage

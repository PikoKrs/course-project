//
// ФАЙЛ flight.hpp
// Рейс: перевозка между двумя аэропортами с фиксированным номером.
// Агрегатор мест багажа: рейс не создаёт места багажа и не уничтожает их,
// он лишь хранит weak_ptr на объекты, которыми владеют другие (агрегация).
//
#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

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
        return AliveCount();
    }

    [[nodiscard]] double GetLoadedWeightKg() const;

    // Назначить место багажа на рейс; проверяется вместимость отсека (правило 7).
    // Рейс не владеет местом: в манифест попадает weak_ptr, жизнь места ему не продлевает
    bool AddPiece(const std::shared_ptr<BaggagePiece>& piece);

    // Снять место багажа с рейса по номеру бирки (сценарий 3 предметной области)
    bool RemovePiece(std::string_view tagNumber);

    // Загрузка отсека: в рейс попадают только места, прошедшие досмотр (правило 5)
    std::size_t LoadAll();

    // Вылет рейса: загруженные места переходят в статус "в пути"
    void Depart();

    void PrintManifest() const;

private:
    // Число записей манифеста, чьи места ещё живы
    [[nodiscard]] std::size_t AliveCount() const;

    std::string m_number;
    std::string m_destination;
    std::size_t m_maxPieces{};
    double m_holdCapacityKg{};
    std::vector<std::weak_ptr<BaggagePiece>> m_pieces; // агрегация: рейс не владеет местами багажа
};

} // namespace baggage

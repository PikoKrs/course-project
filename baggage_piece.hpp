//
// ФАЙЛ baggage_piece.hpp
// Место багажа: физическая единица сдаваемого багажа (чемодан, сумка, коробка).
// Владеет багажной биркой как частью (композиция) и при этом существует
// независимо от рейса: рейс лишь хранит указатель на место (агрегация).
//
#pragma once

#include <string>
#include <string_view>

#include "baggage_tag.hpp"

namespace baggage
{

class BaggagePiece
{
public:
    enum class Status
    {
        eNew = 0,    // не оформлено
        eAccepted,   // принято на рейс
        eScreened,   // прошло досмотр
        eLoaded,     // загружено в багажный отсек
        eInTransit,  // в пути
        eDelivered,  // выдано пассажиру
        eOffloaded   // снято с рейса
    };

    // Предельный вес одного места багажа: ни при какой доплате больше нельзя (правило 4)
    static constexpr double kMaxWeightKg = 32.0;

    BaggagePiece(std::string_view owner, double weightKg, double sizeSumCm, std::string_view tagNumber);

    // Неоформленное место багажа (нужен для динамического массива объектов)
    BaggagePiece();

    BaggagePiece(const BaggagePiece&)            = delete;
    BaggagePiece(BaggagePiece&&)                 = delete;
    BaggagePiece& operator=(const BaggagePiece&) = delete;
    BaggagePiece& operator=(BaggagePiece&&)      = delete;
    ~BaggagePiece();

    [[nodiscard]] std::string GetOwner() const
    {
        return m_owner;
    }

    [[nodiscard]] double GetWeightKg() const
    {
        return m_weightKg;
    }

    [[nodiscard]] double GetSizeSumCm() const
    {
        return m_sizeSumCm;
    }

    [[nodiscard]] std::string GetTagNumber() const
    {
        return m_tag.GetNumber();
    }

    [[nodiscard]] Status GetStatus() const
    {
        return m_status;
    }

    static std::string_view StatusToString(Status status);

    void SetWeightKg(double weightKg);
    void SetSizeSumCm(double sizeSumCm);

    // Приём места багажа при регистрации: normKg — норма провоза билета,
    // feePaid — признак оплаты сбора за сверхнормативный багаж (правила 2-4)
    bool Accept(double normKg, bool feePaid);

    // Досмотр: при обнаружении запрещённого предмета место снимается с рейса (правило 6)
    bool PassScreening(bool prohibitedFound);

    // Загрузка в багажный отсек; возможна только после досмотра (правило 5)
    bool LoadIntoCompartment();

    // Вылет рейса: место переходит в статус "в пути"
    bool MarkInTransit();

    // Выдача после сверки номера отрывной части бирки (правило 10)
    bool Deliver(std::string_view stubNumber);

    // Работа с частью (композиция): считывание бирки через метод целого
    std::string ScanTag() const;

    // Работа с частью (композиция): выдача отрывной части бирки пассажиру
    bool IssueTagStub();

    void PrintInfo() const;

private:
    // Правило 12: статус не может изменяться в обратную сторону по цепочке обработки
    bool ChangeStatus(Status next, std::string_view action);

    std::string m_owner;
    double m_weightKg{};
    double m_sizeSumCm{};
    BaggageTag m_tag{}; // композиция: бирка — неотъемлемая часть места багажа
    Status m_status{ Status::eNew };
};

} // namespace baggage

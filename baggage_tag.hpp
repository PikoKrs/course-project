//
// ФАЙЛ baggage_tag.hpp
// Багажная бирка: уникальный номер, прикрепляемый к месту багажа при регистрации.
// Является частью места багажа (композиция): бирка создаётся вместе с местом
// и уничтожается вместе с ним, отдельно от места не существует.
//
#pragma once

#include <cstddef>
#include <string>
#include <string_view>

namespace baggage
{

class BaggageTag
{
public:
    // Номер бирки — десятизначный код (правило 2 предметной области)
    static constexpr std::size_t kNumberLength = 10;

    // Неоформленная бирка (номер-заглушка, недействительна)
    BaggageTag();

    // Оформление бирки с проверкой формата номера
    explicit BaggageTag(std::string_view number);

    BaggageTag(const BaggageTag&)            = delete;
    BaggageTag(BaggageTag&&)                 = delete;
    BaggageTag& operator=(const BaggageTag&) = delete;
    BaggageTag& operator=(BaggageTag&&)      = delete;
    ~BaggageTag();

    [[nodiscard]] std::string GetNumber() const
    {
        return m_number;
    }

    [[nodiscard]] bool IsValid() const
    {
        return m_valid;
    }

    // Считывание бирки сортировочной системой аэропорта
    std::string Scan() const;

    // Выдача отрывной части бирки пассажиру (выполняется ровно один раз)
    bool IssueStub();

private:
    std::string m_number;
    bool m_valid{};
    bool m_stubIssued{};
};

} // namespace baggage

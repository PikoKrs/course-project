#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

namespace baggage
{

namespace detail
{

inline std::string FormatNumber(double value)
{
    std::ostringstream out;
    out << value;
    return out.str();
}

} // namespace detail

class BaggageRuleError : public std::logic_error
{
public:
    explicit BaggageRuleError(const std::string& message)
        : std::logic_error{ message }
    {
    }
};

class OverweightError final : public BaggageRuleError
{
public:
    explicit OverweightError(double weightKg)
        : BaggageRuleError{ "правило 4: вес места багажа " + detail::FormatNumber(weightKg)
                            + " кг превышает предельные 32 кг, приём невозможен" }
        , m_weightKg{ weightKg }
    {
    }

    [[nodiscard]] double GetWeightKg() const noexcept
    {
        return m_weightKg;
    }

private:
    double m_weightKg;
};

class CheckinClosedError final : public BaggageRuleError
{
public:
    explicit CheckinClosedError(const std::string& flightNumber)
        : BaggageRuleError{ "правило 9: регистрация на рейс " + flightNumber + " закрыта, приём багажа прекращён" }
        , m_flightNumber{ flightNumber }
    {
    }

    [[nodiscard]] const std::string& GetFlightNumber() const noexcept
    {
        return m_flightNumber;
    }

private:
    std::string m_flightNumber;
};

class FlightCapacityError final : public BaggageRuleError
{
public:
    FlightCapacityError(const std::string& flightNumber, const std::string& reason)
        : BaggageRuleError{ "правило 7: рейс " + flightNumber + " не принимает место багажа: " + reason }
        , m_flightNumber{ flightNumber }
    {
    }

    [[nodiscard]] const std::string& GetFlightNumber() const noexcept
    {
        return m_flightNumber;
    }

private:
    std::string m_flightNumber;
};

class NotClearedError final : public BaggageRuleError
{
public:
    explicit NotClearedError(const std::string& tagNumber)
        : BaggageRuleError{ "правила 5-6: место багажа " + tagNumber + " не прошло досмотр, загрузка в рейс запрещена" }
        , m_tagNumber{ tagNumber }
    {
    }

    [[nodiscard]] const std::string& GetTagNumber() const noexcept
    {
        return m_tagNumber;
    }

private:
    std::string m_tagNumber;
};

class NotOnRouteError final : public BaggageRuleError
{
public:
    NotOnRouteError(const std::string& flightNumber, const std::string& ticketRoute)
        : BaggageRuleError{ "правило 8: рейс " + flightNumber + " не входит в маршрут билета " + ticketRoute }
        , m_flightNumber{ flightNumber }
    {
    }

    [[nodiscard]] const std::string& GetFlightNumber() const noexcept
    {
        return m_flightNumber;
    }

private:
    std::string m_flightNumber;
};

class DuplicateTagError final : public BaggageRuleError
{
public:
    explicit DuplicateTagError(const std::string& reason)
        : BaggageRuleError{ "правило 2: " + reason }
    {
    }
};

} // namespace baggage

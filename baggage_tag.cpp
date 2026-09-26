//
// ФАЙЛ baggage_tag.cpp
//
#include "baggage_tag.hpp"

#include <algorithm>
#include <iostream>

namespace baggage
{

namespace
{

bool IsDigits(std::string_view text)
{
    return !text.empty()
        && std::all_of(text.begin(), text.end(), [](unsigned char ch) { return std::isdigit(ch) != 0; });
}

} // namespace

BaggageTag::BaggageTag()
    : m_number{ std::string(kNumberLength, '0') }
{
}

BaggageTag::BaggageTag(std::string_view number)
    : m_number{ std::string(number) }
{
    if (m_number.size() != kNumberLength || !IsDigits(m_number))
    {
        // Правило 2: номер бирки должен состоять ровно из 10 цифр
        std::cout << "[BaggageTag] Ошибка: номер \"" << m_number
                  << "\" должен содержать ровно 10 цифр; бирка недействительна\n";
        m_number = std::string(kNumberLength, '0');
        m_valid  = false;
        return;
    }
    m_valid = true;
    std::cout << "[BaggageTag] Оформлена багажная бирка № " << m_number << "\n";
}

BaggageTag::~BaggageTag()
{
    if (m_valid)
    {
        std::cout << "[~BaggageTag] Багажная бирка № " << m_number << " уничтожена\n";
    }
    else
    {
        std::cout << "[~BaggageTag] Неоформленная багажная бирка уничтожена\n";
    }
}

std::string BaggageTag::Scan() const
{
    if (!m_valid)
    {
        std::cout << "[BaggageTag] Считывание невозможно: бирка не оформлена\n";
        return {};
    }
    std::cout << "[BaggageTag] Сортировочная система считала бирку № " << m_number << "\n";
    return m_number;
}

bool BaggageTag::IssueStub()
{
    if (!m_valid)
    {
        std::cout << "[BaggageTag] Отказ: нельзя выдать отрывную часть неоформленной бирки\n";
        return false;
    }
    if (m_stubIssued)
    {
        // Отрывная часть выдаётся пассажиру ровно один раз
        std::cout << "[BaggageTag] Отказ: отрывная часть бирки № " << m_number << " уже выдана\n";
        return false;
    }
    m_stubIssued = true;
    std::cout << "[BaggageTag] Отрывная часть бирки № " << m_number << " выдана пассажиру\n";
    return true;
}

} // namespace baggage

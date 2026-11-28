#pragma once

#include "base_baggage.hpp"

#include <memory>
#include <string>
#include <vector>

namespace baggage
{

class Flight
{
public:
    Flight(const std::string& number, const std::string& fromAirport, const std::string& toAirport,
           int capacityPlaces, double maxTotalWeightKg);
    Flight(const Flight&)            = delete;
    Flight& operator=(const Flight&) = delete;
    ~Flight();

    bool AddBaggage(const std::shared_ptr<BaseBaggage>& piece);
    bool RemoveBaggage(const std::string& tagNumber);
    void CloseCheckIn();

    static void ParseNumber(const std::string& raw);

    [[nodiscard]] const std::string& GetNumber() const noexcept
    {
        return m_number;
    }

    [[nodiscard]] int LoadedCount() const noexcept
    {
        return static_cast<int>(m_manifest.size());
    }

    void PrintManifest() const;

private:
    [[nodiscard]] double LoadedWeightKg() const noexcept;

    std::string m_number;
    std::string m_fromAirport;
    std::string m_toAirport;
    int m_capacityPlaces;
    double m_maxTotalWeightKg;
    bool m_checkInClosed;
    std::vector<std::weak_ptr<BaseBaggage>> m_manifest;
};

} // namespace baggage

#pragma once

#include "baggage_norm.hpp"
#include "baggage_tag.hpp"

#include <string>

namespace baggage
{

enum class BaggageStatus
{
    eNone = 0,
    eAccepted,
    eScreening,
    eSorted,
    eLoaded,
    eInTransit,
    eIssued,
    eDelayed,
    eLost,
    eRemoved
};

class BaseBaggage
{
public:
    BaseBaggage();
    BaseBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg, double lengthCm,
                double widthCm, double heightCm);
    BaseBaggage(const BaseBaggage&)            = delete;
    BaseBaggage& operator=(const BaseBaggage&) = delete;
    virtual ~BaseBaggage();

    bool AcceptAtCheckIn(const BaggageNorm& norm);
    bool MarkStatus(BaggageStatus newStatus);
    bool VerifyTagAgainstStub(const std::string& stubNumber) const;

    [[nodiscard]] const std::string& GetTagNumber() const noexcept
    {
        return m_tag.GetNumber();
    }

    [[nodiscard]] double GetWeightKg() const noexcept
    {
        return m_weightKg;
    }

    [[nodiscard]] double GetDimsSumCm() const noexcept
    {
        return m_dimsSumCm;
    }

    [[nodiscard]] BaggageStatus GetStatus() const noexcept
    {
        return m_status;
    }

    void SetWeightKg(double weightKg) noexcept
    {
        m_weightKg = weightKg;
    }

    void SetDims(double lengthCm, double widthCm, double heightCm) noexcept;

    [[nodiscard]] virtual double ExcessFee(const BaggageNorm& norm) const = 0;

    [[nodiscard]] virtual std::string Describe() const;

    void PrintState() const;

protected:
    BaggageTag m_tag;
    double m_weightKg;
    double m_dimsSumCm;
    BaggageStatus m_status;

private:
    [[nodiscard]] static const char* StatusName(BaggageStatus status) noexcept;
};

} // namespace baggage

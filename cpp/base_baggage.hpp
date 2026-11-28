#pragma once

#include "baggage_norm.hpp"
#include "baggage_tag.hpp"

#include <memory>
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
    virtual ~BaseBaggage();

    [[nodiscard]] virtual std::unique_ptr<BaseBaggage> Clone() const = 0;

    [[nodiscard]] virtual double ExcessFee(const BaggageNorm& norm) const = 0;

    [[nodiscard]] virtual std::string Describe() const;

    bool AcceptAtCheckIn(const BaggageNorm& norm);
    bool MarkStatus(BaggageStatus newStatus);
    bool VerifyTagAgainstStub(const std::string& stubNumber) const;
    void ReissueDate(const std::string& issueDate);

    [[nodiscard]] const std::string& GetTagNumber() const noexcept;
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

    void PrintState() const;

protected:
    explicit BaseBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg,
                         double dimsSumCm);

    BaseBaggage(const BaseBaggage&)            = default;
    BaseBaggage& operator=(const BaseBaggage&) = default;
    BaseBaggage(BaseBaggage&&) noexcept        = default;
    BaseBaggage& operator=(BaseBaggage&&) noexcept = default;

    std::shared_ptr<BaggageTag> m_tag;
    double m_weightKg;
    double m_dimsSumCm;
    BaggageStatus m_status;

    [[nodiscard]] const std::string& TagIssueDate() const noexcept;

    [[nodiscard]] const BaggageTag& Tag() const noexcept
    {
        return *m_tag;
    }

private:
    [[nodiscard]] static const char* StatusName(BaggageStatus status) noexcept;
};

} // namespace baggage

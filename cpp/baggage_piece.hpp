#pragma once

#include "base_baggage.hpp"

#include <string>

namespace baggage
{

class BaggagePiece
{
public:
    BaggagePiece();
    BaggagePiece(const std::string& tagNumber, const std::string& issueDate, double weightKg, double lengthCm,
                 double widthCm, double heightCm);
    ~BaggagePiece();

    bool AcceptAtCheckIn();
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

    void PrintState() const;

private:
    BaggageTag m_tag;
    double m_weightKg;
    double m_dimsSumCm;
    BaggageStatus m_status;
};

} // namespace baggage

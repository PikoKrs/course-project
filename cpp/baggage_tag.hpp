#pragma once

#include <string>

namespace baggage
{

class BaggageTag
{
public:
    BaggageTag();
    BaggageTag(const std::string& number, const std::string& issueDate);
    ~BaggageTag();

    [[nodiscard]] static std::string GenerateNumber();

    [[nodiscard]] const std::string& GetNumber() const noexcept
    {
        return m_number;
    }

    [[nodiscard]] const std::string& GetIssueDate() const noexcept
    {
        return m_issueDate;
    }

    void ChangeIssueDate(const std::string& issueDate);

    bool VerifyAgainstStub(const std::string& stubNumber) const;
    void PrintInfo() const;

private:
    std::string m_number;
    std::string m_issueDate;
};

} // namespace baggage

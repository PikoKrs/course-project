#include "check_in_desk.hpp"

#include <iostream>

namespace baggage
{

CheckInDesk::CheckInDesk(std::string deskName)
    : m_deskName{ std::move(deskName) }
    , m_feePolicy{ std::make_unique<StandardFeePolicy>() }
{
    std::cout << "[Стойка] Открыта стойка регистрации " << m_deskName << ", политика сбора: " << m_feePolicy->Name()
              << "\n";
}

void CheckInDesk::SetFeePolicy(std::unique_ptr<IFeePolicy> policy)
{
    std::cout << "[Стойка] Стойка " << m_deskName << ": политика сбора сменилась \"" << m_feePolicy->Name()
              << "\" -> \"" << policy->Name() << "\"\n";
    m_feePolicy = std::move(policy);
}

void CheckInDesk::RegisterPiece(BaseBaggage& piece) const
{
    const double fee = m_feePolicy->FeeFor(piece, BaggageNorm{});
    std::cout << "[Стойка] " << m_deskName << ": место багажа " << piece.GetTagNumber() << " (" << piece.GetWeightKg()
              << " кг), сбор по политике \"" << m_feePolicy->Name() << "\": " << fee << " руб.\n";
}

} // namespace baggage

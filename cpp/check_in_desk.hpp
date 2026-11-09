#pragma once

#include "baggage_norm.hpp"
#include "i_fee_policy.hpp"

#include <memory>
#include <string>

namespace baggage
{

class CheckInDesk
{
public:
    explicit CheckInDesk(std::string deskName);

    void SetFeePolicy(std::unique_ptr<IFeePolicy> policy);
    void RegisterPiece(BaseBaggage& piece) const;

private:
    std::string m_deskName;
    std::unique_ptr<IFeePolicy> m_feePolicy;
};

} // namespace baggage

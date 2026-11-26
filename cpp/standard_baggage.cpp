#include "standard_baggage.hpp"

#include <iostream>

namespace baggage
{

StandardBaggage::StandardBaggage()
    : BaseBaggage{}
{
    std::cout << "[Стандарт] Создано обычное место багажа (сбор за перевес: " << kFeePerKg << " руб. за кг)\n";
}

StandardBaggage::StandardBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg,
                                 double lengthCm, double widthCm, double heightCm)
    : BaseBaggage{ tagNumber, issueDate, weightKg, lengthCm, widthCm, heightCm }
{
    std::cout << "[Стандарт] Обычное место багажа по тарифу, сбор за перевес: " << kFeePerKg << " руб. за кг\n";
}

StandardBaggage::StandardBaggage(const std::string& tagNumber, const std::string& issueDate, double weightKg,
                                 double dimsSumCm)
    : BaseBaggage{ tagNumber, issueDate, weightKg, dimsSumCm }
{
    std::cout << "[Стандарт] Обычное место багажа по тарифу, сбор за перевес: " << kFeePerKg << " руб. за кг\n";
}

StandardBaggage::~StandardBaggage() noexcept
{
    std::cout << "[Стандарт] Обычное место багажа " << GetTagNumber() << " уничтожено (деструктор наследника)\n";
}

std::string StandardBaggage::ScanBarcode() const noexcept
{
    std::cout << "[Сканер] Считан штрих-код бирки обычного места: " << GetTagNumber() << "\n";
    return GetTagNumber();
}

std::unique_ptr<BaseBaggage> StandardBaggage::Clone() const
{
    auto clone = std::make_unique<StandardBaggage>(BaggageTag::GenerateNumber(), TagIssueDate(), m_weightKg,
                                                   m_dimsSumCm);
    clone->m_status = m_status;
    std::cout << "[Стандарт] Глубокая копия: у места собственная бирка " << clone->GetTagNumber() << "\n";
    return clone;
}

double StandardBaggage::ExcessFee(const BaggageNorm& norm) const
{
    if (m_weightKg <= norm.GetMaxWeightKg())
    {
        return 0.0;
    }

    return (m_weightKg - norm.GetMaxWeightKg()) * kFeePerKg;
}

std::string StandardBaggage::Describe() const
{
    return BaseBaggage::Describe() + "; обычное место багажа, действует норма тарифа";
}

} // namespace baggage

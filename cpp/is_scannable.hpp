#pragma once

#include <string>

namespace baggage
{

class IScannable
{
public:
    virtual ~IScannable() = default;

    [[nodiscard]] virtual std::string ScanBarcode() const = 0;
};

} // namespace baggage

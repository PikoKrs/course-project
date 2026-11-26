#pragma once

#include "base_baggage.hpp"

#include <vector>

namespace baggage
{

class SortingSystem
{
public:
    void RouteToLoading(BaseBaggage& piece) const;
    void RouteBatchToLoading(const std::vector<std::unique_ptr<BaseBaggage>>& pieces) const;
};

} // namespace baggage

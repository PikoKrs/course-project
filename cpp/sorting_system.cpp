#include "sorting_system.hpp"
#include "is_scannable.hpp"

#include <iostream>

namespace baggage
{

void SortingSystem::RouteToLoading(BaseBaggage& piece) const
{
    const IScannable* scannable = dynamic_cast<const IScannable*>(&piece);
    if (scannable == nullptr)
    {
        std::cout << "[Сортировка] Место багажа " << piece.GetTagNumber()
                  << " не поддерживает IScannable: направляется на ручную обработку\n";
        return;
    }

    const std::string barcode = scannable->ScanBarcode();
    piece.MarkStatus(BaggageStatus::eSorted);
    std::cout << "[Сортировка] Место багажа " << barcode << " направлено к загрузке, статус: отсортировано\n";
}

void SortingSystem::RouteBatchToLoading(const std::vector<std::unique_ptr<BaseBaggage>>& pieces) const
{
    std::cout << "[Сортировка] Начата обработка партии из " << pieces.size() << " мест багажа\n";
    for (const std::unique_ptr<BaseBaggage>& piece : pieces)
    {
        RouteToLoading(*piece);
    }
}

} // namespace baggage

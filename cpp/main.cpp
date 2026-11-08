#include "base_baggage.hpp"
#include "fragile_baggage.hpp"
#include "flight.hpp"
#include "oversize_baggage.hpp"
#include "sorting_system.hpp"
#include "standard_baggage.hpp"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

int main()
{
    std::system("chcp 65001 >nul");

    std::cout << "=== ЛР4. Наследование: иерархия мест багажа ===\n\n";

    std::cout << "--- 1. Норма провоза и разбор строки номера рейса ---\n";
    baggage::BaggageNorm norm;
    baggage::Flight::ParseNumber("S7-402");
    baggage::Flight::ParseNumber("SU-1406");

    std::cout << "\n--- 2. Приёмка рейса: массив unique_ptr<BaseBaggage> ---\n";
    std::cout << "Порядок конструирования: конструктор базы, затем наследника\n";
    std::vector<std::unique_ptr<baggage::BaseBaggage>> loadout;
    loadout.push_back(std::make_unique<baggage::StandardBaggage>("БАГ-240100", "28.10.2026", 18.0, 55.0, 38.0, 22.0));
    loadout.push_back(std::make_unique<baggage::StandardBaggage>("БАГ-240101", "28.10.2026", 26.0, 60.0, 40.0, 25.0));
    loadout.push_back(
        std::make_unique<baggage::OversizeBaggage>("БАГ-240102", "28.10.2026", 22.0, 150.0, 40.0, 25.0, "велосипед",
                                                   180.0));
    loadout.push_back(std::make_unique<baggage::FragileBaggage>("БАГ-240103", "28.10.2026", 12.0, 45.0, 35.0, 25.0));

    std::cout << "\nПриём на стойке через указатель на базовый класс (сбор зависит от типа места):\n";
    for (const std::unique_ptr<baggage::BaseBaggage>& piece : loadout)
    {
        piece->AcceptAtCheckIn(norm);
    }

    std::cout << "\n--- 3. Полиморфное Describe через базовый класс ---\n";
    for (const std::unique_ptr<baggage::BaseBaggage>& piece : loadout)
    {
        std::cout << "- " << piece->Describe() << "\n";
    }

    std::cout << "\n--- 4. Рейс: манифест из мест на shared_ptr (weak_ptr — агрегация) ---\n";
    baggage::Flight flight{ "S7-402", "Барнаул", "Москва", 4, 80.0 };
    std::shared_ptr<baggage::BaseBaggage> checkedFirst = std::make_shared<baggage::StandardBaggage>(
        baggage::BaggageTag::GenerateNumber(), "28.10.2026", 19.5, 50.0, 40.0, 20.0);
    std::shared_ptr<baggage::BaseBaggage> checkedSecond = std::make_shared<baggage::StandardBaggage>(
        baggage::BaggageTag::GenerateNumber(), "28.10.2026", 23.0, 55.0, 40.0, 23.0);
    checkedFirst->AcceptAtCheckIn(norm);
    checkedSecond->AcceptAtCheckIn(norm);
    flight.AddBaggage(checkedFirst);
    flight.AddBaggage(checkedSecond);
    flight.PrintManifest();
    std::cout << "Номера бирок для мест рейса выданы статическим генератором BaggageTag::GenerateNumber\n";

    std::cout << "\n--- 5. Правило 12 через базовый класс ---\n";
    loadout[0]->MarkStatus(baggage::BaggageStatus::eScreening);
    loadout[0]->MarkStatus(baggage::BaggageStatus::eAccepted);

    std::cout << "\n--- 6. Поверхностная копия: копия делит бирку с оригиналом ---\n";
    baggage::StandardBaggage shallowOrigin{ "БАГ-240115", "28.10.2026", 20.0, 100.0 };
    shallowOrigin.AcceptAtCheckIn(norm);
    baggage::StandardBaggage shallowCopy{ shallowOrigin };
    std::cout << "Оригинал:  " << shallowOrigin.Describe() << "\n";
    std::cout << "Копия:     " << shallowCopy.Describe() << "\n";
    std::cout << "Номера бирок совпадают: правило 2 (уникальность) нарушено поверхностной копией\n";
    shallowCopy.ReissueDate("29.10.2026");
    std::cout << "После ReissueDate у копии оригинал тоже видит новую дату:\n";
    std::cout << "Оригинал:  " << shallowOrigin.Describe() << "\n";

    std::cout << "\n--- 7. Глубокая копия: Clone с собственной биркой ---\n";
    std::unique_ptr<baggage::BaseBaggage> deepCopy = shallowOrigin.Clone();
    deepCopy->ReissueDate("30.10.2026");
    std::cout << "После ReissueDate у клона оригинал не изменился:\n";
    std::cout << "Оригинал:  " << shallowOrigin.Describe() << "\n";
    std::cout << "Клон:      " << deepCopy->Describe() << "\n";

    std::cout << "\n--- 8. OversizeBaggage: правило пяти (копирование и перемещение) ---\n";
    const baggage::OversizeBaggage& oversize = static_cast<const baggage::OversizeBaggage&>(*loadout[2]);
    baggage::OversizeBaggage oversizeCopy{ oversize };
    oversizeCopy = oversizeCopy;
    std::cout << "Копия негабарита: " << oversizeCopy.Describe() << "\n";
    baggage::OversizeBaggage oversizeMoved{ std::move(oversizeCopy) };
    std::cout << "После перемещения: " << oversizeMoved.Describe() << "\n";

    std::cout << "\n--- 9. Хрупкое место: пометка о повреждении (сценарий 5) ---\n";
    auto* fragile = static_cast<baggage::FragileBaggage*>(loadout[3].get());
    fragile->MarkStatus(baggage::BaggageStatus::eLoaded);
    fragile->MarkStatus(baggage::BaggageStatus::eInTransit);
    fragile->MarkStatus(baggage::BaggageStatus::eIssued);
    fragile->MarkDamaged();
    std::cout << fragile->Describe() << "\n";

    std::cout << "\n--- 10. Сортировочная система через интерфейс IScannable ---\n";
    baggage::SortingSystem sorting;
    sorting.RouteBatchToLoading(loadout);
    std::cout << "FragileBaggage не реализует IScannable — dynamic_cast вернул nullptr, место ушло на ручную обработку\n";

    std::cout << "\n--- 11. Уничтожение наследника через указатель на базовый класс ---\n";
    std::cout << "loadout.back().reset(): вызван деструктор наследника, затем базы (виртуальный деструктор)\n";
    loadout.back().reset();
    std::cout << "\nКлон и копии уничтожаются при выходе из main — порядок: наследник, потом база\n";
    std::cout << "=== Демонстрация завершена ===\n";
    return 0;
}
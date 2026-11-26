#include "base_baggage.hpp"
#include "baggage_rule_error.hpp"
#include "check_in_desk.hpp"
#include "fee_policies.hpp"
#include "fragile_baggage.hpp"
#include "flight.hpp"
#include "oversize_baggage.hpp"
#include "sorting_system.hpp"
#include "standard_baggage.hpp"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

namespace
{

void ShowCaught(const baggage::BaggageRuleError& error)
{
    std::cout << "  Перехвачено BaggageRuleError: " << error.what() << "\n";
}

} // namespace

int main()
{
    std::system("chcp 65001 >nul");

    std::cout << "=== ЛР5. Полиморфизм: интерфейсы, конфигурирование, исключения ===\n\n";

    std::cout << "--- 1. Норма провоза и разбор номера рейса ---\n";
    baggage::BaggageNorm norm;
    baggage::Flight::ParseNumber("S7-402");

    std::cout << "\n--- 2. Приёмка: успех и OverweightError (правило 4) ---\n";
    std::vector<std::unique_ptr<baggage::BaseBaggage>> loadout;
    loadout.push_back(std::make_unique<baggage::StandardBaggage>("БАГ-240100", "09.11.2026", 18.0, 55.0, 38.0, 22.0));
    loadout.push_back(std::make_unique<baggage::OversizeBaggage>("БАГ-240101", "09.11.2026", 22.0, 150.0, 40.0, 25.0,
                                                                 "велосипед", 180.0));
    loadout.push_back(std::make_unique<baggage::FragileBaggage>("БАГ-240102", "09.11.2026", 12.0, 45.0, 35.0, 25.0));
    try
    {
        for (const std::unique_ptr<baggage::BaseBaggage>& piece : loadout)
        {
            piece->AcceptAtCheckIn(norm);
        }
        std::cout << "  Все три места приняты без нарушений\n";
    }
    catch (const baggage::OverweightError& error)
    {
        std::cout << "  Перехвачен OverweightError (вес " << error.GetWeightKg() << " кг): " << error.what() << "\n";
    }
    catch (const baggage::BaggageRuleError& error)
    {
        ShowCaught(error);
    }

    try
    {
        std::cout << "  Попытка приёма места весом 35 кг:\n";
        baggage::StandardBaggage heavy{ "БАГ-240103", "09.11.2026", 35.0, 60.0, 45.0, 30.0 };
        heavy.AcceptAtCheckIn(norm);
    }
    catch (const baggage::OverweightError& error)
    {
        std::cout << "  Перехвачен OverweightError (вес " << error.GetWeightKg() << " кг): " << error.what() << "\n";
    }
    catch (const baggage::BaggageRuleError& error)
    {
        ShowCaught(error);
    }

    std::cout << "\n--- 3. Раннее и позднее связывание ---\n";
    baggage::BaseBaggage& piece = *loadout[0];
    std::cout << "Невиртуальный GetWeightKg() по указателю BaseBaggage* (раннее связывание, тип указателя — база): "
              << piece.GetWeightKg() << " кг\n";
    std::cout << "Виртуальный Describe() по той же ссылке (позднее связывание, тип объекта — StandardBaggage):\n  "
              << piece.Describe() << "\n";

    std::cout << "\n--- 4. Рейс: AddBaggage, CheckinClosedError, FlightCapacityError ---\n";
    baggage::Flight flight{ "S7-402", "Барнаул", "Москва", 2, 80.0 };
    std::shared_ptr<baggage::BaseBaggage> checked = std::make_shared<baggage::StandardBaggage>(
        baggage::BaggageTag::GenerateNumber(), "09.11.2026", 19.5, 50.0, 40.0, 20.0);
    checked->AcceptAtCheckIn(norm);
    try
    {
        flight.AddBaggage(checked);
        std::cout << "  Успех: место назначено на рейс\n";

        flight.CloseCheckIn();
        std::cout << "  Попытка добавить место после закрытия регистрации:\n";
        flight.AddBaggage(checked);
    }
    catch (const baggage::CheckinClosedError& error)
    {
        std::cout << "  Перехвачен CheckinClosedError (рейс " << error.GetFlightNumber() << "): " << error.what()
                  << "\n";
    }
    catch (const baggage::BaggageRuleError& error)
    {
        ShowCaught(error);
    }

    baggage::Flight capacityProbe{ "S7-999", "Барнаул", "Томск", 1, 80.0 };
    std::shared_ptr<baggage::BaseBaggage> firstPlace = std::make_shared<baggage::StandardBaggage>(
        baggage::BaggageTag::GenerateNumber(), "09.11.2026", 15.0, 60.0, 40.0, 20.0);
    std::shared_ptr<baggage::BaseBaggage> secondPlace = std::make_shared<baggage::StandardBaggage>(
        baggage::BaggageTag::GenerateNumber(), "09.11.2026", 16.0, 60.0, 40.0, 20.0);
    try
    {
        capacityProbe.AddBaggage(firstPlace);
        std::cout << "  Попытка добавить второе место в отсек на 1 место:\n";
        capacityProbe.AddBaggage(secondPlace);
    }
    catch (const baggage::FlightCapacityError& error)
    {
        std::cout << "  Перехвачен FlightCapacityError (рейс " << error.GetFlightNumber() << "): " << error.what()
                  << "\n";
    }
    catch (const baggage::BaggageRuleError& error)
    {
        ShowCaught(error);
    }

    std::cout << "\n--- 5. NotOnRouteError (правило 8) ---\n";
    try
    {
        flight.Assign(*loadout[0], "S7-402 / S7-210");
        std::cout << "  Попытка назначить место на рейс вне маршрута билета:\n";
        baggage::Flight wrongFlight{ "S7-777", "Новосибирск", "Сочи", 3, 90.0 };
        wrongFlight.Assign(*loadout[0], "S7-402 / S7-210");
    }
    catch (const baggage::NotOnRouteError& error)
    {
        std::cout << "  Перехвачен NotOnRouteError (рейс " << error.GetFlightNumber() << "): " << error.what() << "\n";
    }
    catch (const baggage::BaggageRuleError& error)
    {
        ShowCaught(error);
    }

    std::cout << "\n--- 6. NotClearedError (правила 5-6): загрузка без досмотра ---\n";
    try
    {
        flight.LoadManifest();
    }
    catch (const baggage::NotClearedError& error)
    {
        std::cout << "  Перехвачен NotClearedError (место " << error.GetTagNumber() << "): " << error.what() << "\n";
    }
    catch (const baggage::BaggageRuleError& error)
    {
        ShowCaught(error);
    }

    std::cout << "  Место багажа " << checked->GetTagNumber() << " проходит досмотр и сортировку, повторная загрузка:\n";
    checked->MarkStatus(baggage::BaggageStatus::eScreening);
    checked->MarkStatus(baggage::BaggageStatus::eSorted);
    try
    {
        flight.LoadManifest();
    }
    catch (const baggage::NotClearedError& error)
    {
        std::cout << "  Перехвачен NotClearedError: " << error.what() << "\n";
    }
    catch (const baggage::BaggageRuleError& error)
    {
        ShowCaught(error);
    }

    std::cout << "\n--- 7. DuplicateTagError (правило 2) ---\n";
    try
    {
        std::cout << "  Попытка оформить бирку с уже выданным номером БАГ-240100:\n";
        baggage::BaggageTag duplicate{ "БАГ-240100", "09.11.2026" };
        duplicate.PrintInfo();
    }
    catch (const baggage::DuplicateTagError& error)
    {
        std::cout << "  Перехвачен DuplicateTagError: " << error.what() << "\n";
    }
    catch (const baggage::BaggageRuleError& error)
    {
        ShowCaught(error);
    }

    std::cout << "\n--- 8. Общий обработчик BaggageRuleError (правило 12 через базовый класс) ---\n";
    try
    {
        loadout[1]->MarkStatus(baggage::BaggageStatus::eAccepted);
    }
    catch (const baggage::BaggageRuleError& error)
    {
        std::cout << "  Частный тип не указан — сработал общий обработчик иерархии:\n";
        ShowCaught(error);
    }

    std::cout << "\n--- 9. Сортировка через IScannable (dynamic_cast) ---\n";
    baggage::SortingSystem sorting;
    sorting.RouteBatchToLoading(loadout);
    std::cout << "FragileBaggage не реализует IScannable — dynamic_cast вернул nullptr, место ушло на ручную обработку\n";

    std::cout << "\n--- 10. Динамическое конфигурирование политики сбора ---\n";
    baggage::CheckInDesk desk{ "N1" };
    baggage::StandardBaggage feeProbe{ "БАГ-240104", "09.11.2026", 26.0, 120.0 };
    feeProbe.AcceptAtCheckIn(norm);
    desk.RegisterPiece(feeProbe);
    desk.SetFeePolicy(std::make_unique<baggage::PromoFeePolicy>());
    desk.RegisterPiece(feeProbe);
    desk.SetFeePolicy(std::make_unique<baggage::OversizeFeePolicy>());
    desk.RegisterPiece(feeProbe);

    std::cout << "\n=== Демонстрация завершена; деструкторы и перемещающие операции — noexcept ===\n";
    return 0;
}

#include "baggage_piece.hpp"
#include "flight.hpp"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

void HandleIssued(std::shared_ptr<baggage::BaggagePiece> piece)
{
    std::cout << "[Выдача] Место багажа " << piece->GetTagNumber() << " передано на ленту выдачи, владельцев: "
              << piece.use_count() << " (рейс держит weak_ptr и счётчик не увеличивает)\n";
    piece->PrintState();
}

int main()
{
    std::system("chcp 65001 >nul");

    std::cout << "=== ЛР3 ч.2. Сценарий 1: приём багажа на рейс (умные указатели) ===\n\n";

    std::cout << "--- 1. Рейс и приём мест в норме ---\n";
    baggage::Flight flight{ "S7-402", "Барнаул", "Москва", 2, 40.0 };

    std::shared_ptr<baggage::BaggagePiece> first = std::make_shared<baggage::BaggagePiece>("БАГ-240001", "15.10.2026", 23.5, 55.0, 38.0, 22.0);
    first->AcceptAtCheckIn();
    std::cout << "[Место] Владельцев shared_ptr у БАГ-240001: " << first.use_count() << "\n";
    flight.AddBaggage(first);
    std::cout << "[Место] После назначения на рейс: " << first.use_count()
              << " (манифест рейса держит weak_ptr — агрегация, счётчик владельцев не меняется)\n";

    std::shared_ptr<baggage::BaggagePiece> second = std::make_shared<baggage::BaggagePiece>("БАГ-240002", "15.10.2026", 18.0, 60.0, 40.0, 25.0);
    second->AcceptAtCheckIn();
    flight.AddBaggage(second);

    std::cout << "\n--- 2. Нарушение правила 7: суммарный вес ---\n";
    std::shared_ptr<baggage::BaggagePiece> third = std::make_shared<baggage::BaggagePiece>("БАГ-240003", "15.10.2026", 20.0, 45.0, 35.0, 25.0);
    third->AcceptAtCheckIn();
    flight.AddBaggage(third);

    std::shared_ptr<baggage::BaggagePiece> thirdLight = std::make_shared<baggage::BaggagePiece>("БАГ-240004", "15.10.2026", 1.5, 30.0, 20.0, 15.0);
    thirdLight->AcceptAtCheckIn();
    flight.AddBaggage(thirdLight);

    std::cout << "\n--- 3. Нарушение правила 4: перевес 35 кг ---\n";
    std::shared_ptr<baggage::BaggagePiece> heavy = std::make_shared<baggage::BaggagePiece>("БАГ-240005", "15.10.2026", 35.0, 60.0, 45.0, 30.0);
    heavy->AcceptAtCheckIn();

    std::cout << "\n--- 4. Нарушение правила 7: манифест заполнен ---\n";
    std::shared_ptr<baggage::BaggagePiece> fourth = std::make_shared<baggage::BaggagePiece>("БАГ-240006", "15.10.2026", 9.0, 40.0, 30.0, 20.0);
    fourth->AcceptAtCheckIn();
    flight.AddBaggage(fourth);

    std::cout << "\n--- 5. Нарушение правила 9: регистрация закрыта ---\n";
    flight.CloseCheckIn();
    std::shared_ptr<baggage::BaggagePiece> late = std::make_shared<baggage::BaggagePiece>("БАГ-240007", "15.10.2026", 12.0, 50.0, 40.0, 30.0);
    late->AcceptAtCheckIn();
    flight.AddBaggage(late);

    std::cout << "\n--- 6. Манифест рейса после приёма ---\n";
    flight.PrintManifest();

    std::cout << "\n--- 7. unique_ptr: уничтожение при выходе из блока без delete ---\n";
    std::cout << "Вход во вложенный блок\n";
    {
        std::unique_ptr<baggage::BaggagePiece> temp = std::make_unique<baggage::BaggagePiece>("БАГ-240010", "16.10.2026", 21.0, 50.0, 40.0, 20.0);
        temp->AcceptAtCheckIn();
        temp->PrintState();
    }
    std::cout << "Выход из вложенного блока: место и его бирка уничтожены владельцем unique_ptr, delete не вызывался\n";

    std::cout << "\n--- 8. unique_ptr: передача владения через std::move ---\n";
    std::unique_ptr<baggage::BaggagePiece> owner = std::make_unique<baggage::BaggagePiece>("БАГ-240012", "16.10.2026", 27.0, 60.0, 45.0, 25.0);
    owner->AcceptAtCheckIn();
    std::unique_ptr<baggage::BaggagePiece> movedTo = std::move(owner);
    std::cout << "После std::move исходный указатель пуст: " << (owner == nullptr ? "да" : "нет")
              << ", место у нового владельца: " << movedTo->GetTagNumber() << "\n";

    std::cout << "\n--- 9. shared_ptr: счётчик владельцев и уничтожение с последним владельцем ---\n";
    std::shared_ptr<baggage::BaggagePiece> single = std::make_shared<baggage::BaggagePiece>("БАГ-240040", "16.10.2026", 19.5, 45.0, 35.0, 20.0);
    single->AcceptAtCheckIn();
    std::cout << "Владельцев после создания: " << single.use_count() << "\n";
    {
        std::shared_ptr<baggage::BaggagePiece> copy = single;
        std::cout << "Владельцев после копирования: " << single.use_count() << "\n";
    }
    std::cout << "Владельцев после выхода копии из блока: " << single.use_count() << "\n";
    single = nullptr;
    std::cout << "Последний владелец уничтожен — место БАГ-240040 уничтожено вместе с ним (см. сообщения выше)\n";

    std::cout << "\n--- 10. weak_ptr: наблюдение без владения ---\n";
    baggage::Flight survivor{ "S7-333", "Барнаул", "Санкт-Петербург", 3, 100.0 };
    {
        std::shared_ptr<baggage::BaggagePiece> soon = std::make_shared<baggage::BaggagePiece>("БАГ-240041", "16.10.2026", 14.0, 50.0, 40.0, 25.0);
        soon->AcceptAtCheckIn();
        survivor.AddBaggage(soon);
        std::cout << "Место в манифесте, владелец ещё жив: " << soon.use_count() << "\n";
    }
    std::cout << "Владелец вышел из блока, место уничтожено. Печать манифеста:\n";
    survivor.PrintManifest();

    std::cout << "\n--- 11. Динамический массив объектов: std::vector ---\n";
    std::vector<baggage::BaggagePiece> batch;
    batch.reserve(3);
    batch.emplace_back("БАГ-240051", "16.10.2026", 10.0, 40.0, 30.0, 20.0);
    batch.emplace_back("БАГ-240052", "16.10.2026", 31.0, 55.0, 40.0, 25.0);
    batch.emplace_back("БАГ-240053", "16.10.2026", 22.5, 50.0, 40.0, 30.0);
    for (baggage::BaggagePiece& piece : batch)
    {
        piece.AcceptAtCheckIn();
        piece.PrintState();
    }

    std::cout << "\n--- 12. Массив динамических объектов: vector<unique_ptr> ---\n";
    std::vector<std::unique_ptr<baggage::BaggagePiece>> oversized;
    oversized.push_back(std::make_unique<baggage::BaggagePiece>("БАГ-240061", "16.10.2026", 19.0, 70.0, 50.0, 30.0));
    oversized.push_back(std::make_unique<baggage::BaggagePiece>("БАГ-240062", "16.10.2026", 30.5, 80.0, 50.0, 28.0));
    for (const std::unique_ptr<baggage::BaggagePiece>& piece : oversized)
    {
        piece->AcceptAtCheckIn();
        piece->PrintState();
    }
    std::cout << "Вектор уничтожит все места сам при выходе из main — new/delete не используются\n";

    std::cout << "\n--- 13. Правило 12 и выдача: общий доступ через shared_ptr ---\n";
    first->MarkStatus(baggage::BaggageStatus::eSorted);
    first->MarkStatus(baggage::BaggageStatus::eAccepted);
    first->MarkStatus(baggage::BaggageStatus::eLoaded);
    first->MarkStatus(baggage::BaggageStatus::eInTransit);
    HandleIssued(first);
    first->MarkStatus(baggage::BaggageStatus::eIssued);
    std::cout << "Владельцев после возврата из HandleIssued: " << first.use_count() << "\n";
    first->VerifyTagAgainstStub("БАГ-240001");

    std::cout << "\n=== Демонстрация завершена, выход из main: уничтожение оставшихся объектов ===\n";
    return 0;
}

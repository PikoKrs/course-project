#include "baggage_piece.hpp"
#include "flight.hpp"

#include <cstdlib>
#include <iostream>
#include <memory>

int main()
{
    std::system("chcp 65001 >nul");

    std::cout << "=== ЛР3 ч.2. Сценарий 1: приём багажа на рейс (умные указатели) ===\n\n";

    std::cout << "--- 1. Рейс и приём мест в норме ---\n";
    baggage::Flight flight{ "S7-402", "Барнаул", "Москва", 2, 40.0 };

    std::shared_ptr<baggage::BaggagePiece> first = std::make_shared<baggage::BaggagePiece>("БАГ-240001", "15.10.2026", 23.5, 55.0, 38.0, 22.0);
    first->AcceptAtCheckIn();
    flight.AddBaggage(first);

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

    std::cout << "\n--- 7. Композиция: часть умирает вместе с целым ---\n";
    std::cout << "Вход во вложенный блок\n";
    {
        baggage::BaggagePiece temp{ "БАГ-240010", "16.10.2026", 21.0, 50.0, 40.0, 20.0 };
        temp.AcceptAtCheckIn();
        temp.PrintState();
    }
    std::cout << "Выход из вложенного блока: место и его бирка уничтожены вместе\n";

    std::cout << "\n--- 8. Агрегация: место переживает рейс ---\n";
    std::shared_ptr<baggage::BaggagePiece> kept = std::make_shared<baggage::BaggagePiece>("БАГ-240011", "16.10.2026", 24.0, 55.0, 40.0, 23.0);
    kept->AcceptAtCheckIn();
    {
        baggage::Flight local{ "S7-210", "Барнаул", "Новосибирск", 5, 300.0 };
        local.AddBaggage(kept);
        local.PrintManifest();
    }
    std::cout << "Рейс S7-210 уничтожен, место багажа продолжает существовать:\n";
    kept->PrintState();

    std::cout << "\n--- 9. Динамическое создание: new/delete ---\n";
    baggage::BaggagePiece* dynamic = new baggage::BaggagePiece{ "БАГ-240020", "16.10.2026", 26.5, 65.0, 45.0, 25.0 };
    dynamic->AcceptAtCheckIn();
    dynamic->PrintState();
    delete dynamic;

    std::cout << "\n--- 10. Динамический массив объектов ---\n";
    baggage::BaggagePiece* batch = new baggage::BaggagePiece[3];
    batch[0].SetWeightKg(10.0);
    batch[0].SetDims(40.0, 30.0, 20.0);
    batch[1].SetWeightKg(31.0);
    batch[1].SetDims(55.0, 40.0, 25.0);
    batch[2].SetWeightKg(22.5);
    batch[2].SetDims(50.0, 40.0, 30.0);
    for (int i = 0; i < 3; ++i)
    {
        batch[i].AcceptAtCheckIn();
        batch[i].PrintState();
    }
    delete[] batch;

    std::cout << "\n--- 11. Массив динамических объектов ---\n";
    baggage::BaggagePiece** oversized = new baggage::BaggagePiece*[2];
    oversized[0] = new baggage::BaggagePiece{ "БАГ-240031", "16.10.2026", 19.0, 70.0, 50.0, 30.0 };
    oversized[1] = new baggage::BaggagePiece{ "БАГ-240032", "16.10.2026", 30.5, 80.0, 50.0, 28.0 };
    for (int i = 0; i < 2; ++i)
    {
        oversized[i]->AcceptAtCheckIn();
        oversized[i]->PrintState();
    }
    delete oversized[0];
    delete oversized[1];
    delete[] oversized;

    std::cout << "\n--- 12. Сверка бирки с отрывной частью при выдаче ---\n";
    first->MarkStatus(baggage::BaggageStatus::eLoaded);
    first->MarkStatus(baggage::BaggageStatus::eInTransit);
    first->MarkStatus(baggage::BaggageStatus::eIssued);
    first->PrintState();

    std::cout << "\n=== Демонстрация завершена, выход из main: уничтожение оставшихся объектов ===\n";
    return 0;
}

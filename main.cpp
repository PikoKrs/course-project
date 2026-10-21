//
// ФАЙЛ main.cpp
// Демонстрационная программа: фрагмент сценария "полный цикл обработки багажа
// на рейсе" из описания предметной области курсового проекта.
// Тема: автоматизированная система контроля и обработки багажа авиапассажиров.
// В проекте нет ни одного new и delete: объектами владеют умные указатели.
//
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

#include "baggage_piece.hpp"
#include "baggage_tag.hpp"
#include "flight.hpp"

namespace
{

// Работа со статическим объектом по ссылке:
// ссылка допустима как параметр функции, которая объектом не владеет
void InspectByRef(const baggage::BaggagePiece& piece)
{
    std::cout << "  [Осмотр по ссылке] " << piece.GetOwner() << ": " << piece.GetWeightKg() << " кг, бирка "
              << piece.GetTagNumber() << "\n";
}

// Работа с объектом по указателю: указатель-параметр объектом не владеет
// и нигде не сохраняется
void InspectByPointer(const baggage::BaggagePiece* piece)
{
    if (piece == nullptr)
    {
        std::cout << "  [Осмотр по указателю] пустой указатель\n";
        return;
    }
    std::cout << "  [Осмотр по указателю] " << piece->GetOwner() << ": " << piece->GetWeightKg() << " кг, бирка "
              << piece->GetTagNumber() << "\n";
}

} // namespace

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    std::cout << "=== Лабораторная работа 3, часть 2. Умные указатели ===\n";
    std::cout << "Тема: система контроля и обработки багажа авиапассажиров\n";

    // --- 1. Местами багажа владеет shared_ptr: манифест рейса хранит на них weak_ptr ---
    std::cout << "\n--- 1. Приём багажа на регистрации ---\n";
    std::shared_ptr<baggage::BaggagePiece> ivanov =
        std::make_shared<baggage::BaggagePiece>("Иванов И. И.", 22.0, 148.0, "7701234567");
    std::shared_ptr<baggage::BaggagePiece> petrov =
        std::make_shared<baggage::BaggagePiece>("Петров П. П.", 25.5, 151.0, "7701234568");
    std::shared_ptr<baggage::BaggagePiece> kuznetsov =
        std::make_shared<baggage::BaggagePiece>("Кузнецов К. К.", 15.0, 120.0, "7701234570");
    std::shared_ptr<baggage::BaggagePiece> heavy =
        std::make_shared<baggage::BaggagePiece>("Сидоров С. С.", 33.5, 160.0, "7701234569");

    std::cout << "Корректное действие: вес в норме\n";
    ivanov->Accept(23.0, false);
    std::cout << "Нарушение: превышение нормы провоза без доплаты (правило 3)\n";
    petrov->Accept(23.0, false);
    std::cout << "Исправление: сбор за сверхнорматив оплачен\n";
    petrov->Accept(23.0, true);
    std::cout << "Нарушение: вес выше предельных 32 кг даже с доплатой (правило 4)\n";
    heavy->Accept(40.0, true);
    std::cout << "Корректное действие: третье место принято, но ещё не прошло досмотр\n";
    kuznetsov->Accept(23.0, false);

    // --- 2. Композиция: бирка по-прежнему поле-объект, целое работает с ней через методы ---
    std::cout << "\n--- 2. Композиция: обращение к части через методы целого ---\n";
    ivanov->ScanTag();
    ivanov->IssueTagStub();
    std::cout << "Повторная выдача отрывной части должна быть отклонена:\n";
    ivanov->IssueTagStub();

    // --- 3. Агрегация: рейс хранит weak_ptr на места, которыми владеют другие ---
    std::cout << "\n--- 3. Агрегация: назначение мест багажа на рейс ---\n";
    baggage::Flight flight{ "SU 1602", "Барнаул", 5, 65.0 };
    flight.AddPiece(ivanov);
    flight.AddPiece(petrov);
    flight.AddPiece(kuznetsov);
    std::cout << "Нарушение: превышение вместимости отсека (правило 7)\n";
    flight.AddPiece(heavy);
    flight.PrintManifest();

    // --- 4. Досмотр, загрузка и вылет ---
    std::cout << "\n--- 4. Досмотр и загрузка багажного отсека ---\n";
    std::cout << "Нарушение: запрещённый предмет в багаже (правило 6)\n";
    petrov->PassScreening(true);
    ivanov->PassScreening(false);
    std::cout << "Снятие проблемного места с рейса (сценарий 3):\n";
    flight.RemovePiece("7701234568");
    std::cout << "Попытка загрузить место, не прошедшее досмотр (правило 5):\n";
    flight.LoadAll();
    flight.Depart();

    // --- 5. Умные указатели ---
    std::cout << "\n--- 5. Работа с памятью: умные указатели ---\n";

    std::cout << "5.1. std::unique_ptr: один владелец, уничтожение без delete:\n";
    {
        std::unique_ptr<baggage::BaggagePiece> temporary =
            std::make_unique<baggage::BaggagePiece>("Временное место", 12.0, 100.0, "7701234571");
        temporary->Accept(23.0, false);
        std::unique_ptr<baggage::BaggagePiece> accepted = std::move(temporary);
        if (!temporary)
        {
            std::cout << "  temporary пуст: владение передано через std::move\n";
        }
        std::cout << "Выход из вложенного блока:\n";
    } // объект уничтожен здесь, delete не нужен
    std::cout << "Блок завершён\n";

    std::cout << "5.2. Доступ по ссылке и по указателю (параметры без владения):\n";
    InspectByRef(*ivanov);
    InspectByPointer(petrov.get());

    std::cout << "5.3. Массив объектов: std::unique_ptr<BaggagePiece[]>:\n";
    {
        std::unique_ptr<baggage::BaggagePiece[]> batch = std::make_unique<baggage::BaggagePiece[]>(3);
        batch[0].SetWeightKg(10.0);
        batch[0].SetSizeSumCm(115.0);
        batch[1].SetWeightKg(20.0);
        batch[1].SetSizeSumCm(135.0);
        std::cout << "Выход из вложенного блока:\n";
    }
    std::cout << "Блок завершён\n";

    std::cout << "5.4. Массив владельцев: std::vector<std::unique_ptr<BaggagePiece>>:\n";
    {
        std::vector<std::unique_ptr<baggage::BaggagePiece>> parcel;
        parcel.push_back(std::make_unique<baggage::BaggagePiece>("Грузинов Г. Г.", 9.0, 95.0, "7701234572"));
        std::cout << "Нарушение: неверный формат номера бирки (правило 2):\n";
        parcel.push_back(std::make_unique<baggage::BaggagePiece>("Ошибкин О. О.", 8.0, 90.0, "AB-12"));
        parcel[0]->Accept(23.0, false);
        std::cout << "Место без действующей бирки к перевозке не принимается:\n";
        parcel[1]->Accept(23.0, false);
        std::cout << "Выход из вложенного блока:\n";
    }
    std::cout << "Блок завершён\n";

    std::cout << "5.5. std::shared_ptr: счётчик владельцев:\n";
    std::shared_ptr<baggage::BaggagePiece> shared =
        std::make_shared<baggage::BaggagePiece>("Общее место", 16.0, 118.0, "7701234573");
    std::cout << "  Владельцев: " << shared.use_count() << "\n";
    {
        std::shared_ptr<baggage::BaggagePiece> copy = shared;
        std::cout << "  Копия указателя создана, владельцев: " << shared.use_count() << "\n";
    }
    std::cout << "  Копия вышла из блока, владельцев: " << shared.use_count() << "\n";
    std::cout << "  Последний владелец отказывается от объекта:\n";
    shared.reset();

    std::cout << "5.6. std::weak_ptr: наблюдатель не продлевает жизнь объекту:\n";
    std::shared_ptr<baggage::BaggagePiece> observed =
        std::make_shared<baggage::BaggagePiece>("Наблюдаемое место", 11.0, 105.0, "7701234574");
    std::weak_ptr<baggage::BaggagePiece> watcher = observed;
    if (std::shared_ptr<baggage::BaggagePiece> locked = watcher.lock())
    {
        std::cout << "  Объект жив: " << locked->GetOwner() << ", бирка " << locked->GetTagNumber() << "\n";
    }
    std::cout << "  Последний владелец отказывается от объекта:\n";
    observed.reset();
    if (watcher.lock())
    {
        std::cout << "  Объект всё ещё жив\n";
    }
    else
    {
        std::cout << "  lock() вернул пустой указатель: объект уничтожен, программа корректно это обрабатывает\n";
    }

    // --- 6. Время жизни: композиция ---
    std::cout << "\n--- 6. Композиция: уничтожение целого уничтожает часть ---\n";
    std::cout << "Вход во вложенный блок:\n";
    {
        baggage::BaggagePiece local{ "Локалов Л. Л.", 18.0, 125.0, "7701234575" };
        local.Accept(20.0, false);
        std::cout << "Выход из вложенного блока:\n";
    }
    std::cout << "Блок завершён\n";

    // --- 7. Время жизни: агрегация ---
    std::cout << "\n--- 7. Агрегация: уничтожение агрегатора сохраняет объект ---\n";
    std::shared_ptr<baggage::BaggagePiece> survivor =
        std::make_shared<baggage::BaggagePiece>("Живучиков Ж. Ж.", 20.0, 130.0, "7701234576");
    std::cout << "Вход во вложенный блок:\n";
    {
        baggage::Flight shortLived{ "SU 9999", "Томск", 3, 60.0 };
        shortLived.AddPiece(survivor);
        std::cout << "Выход из вложенного блока:\n";
    }
    std::cout << "Объект-агрегатор уничтожен; место багажа живо, владельцев: " << survivor.use_count() << "\n";
    survivor->PrintInfo();
    survivor->ScanTag();
    std::cout << "Последний владелец отказывается от объекта:\n";
    survivor.reset();

    // --- 8. Выдача багажа в аэропорту назначения ---
    std::cout << "\n--- 8. Выдача багажа после прилёта ---\n";
    std::cout << "Нарушение: неверный номер отрывной части (правило 10)\n";
    ivanov->Deliver("1111111111");
    std::cout << "Корректное действие: номера сверены\n";
    ivanov->Deliver("7701234567");

    std::cout << "\n=== Конец демонстрации (объекты уничтожают их умные указатели) ===\n";
    return 0;
}

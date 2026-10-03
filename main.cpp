//
// ФАЙЛ main.cpp
// Демонстрационная программа: фрагмент сценария "полный цикл обработки багажа
// на рейсе" из описания предметной области курсового проекта.
// Тема: автоматизированная система контроля и обработки багажа авиапассажиров.
//
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#include "baggage_piece.hpp"
#include "baggage_tag.hpp"
#include "flight.hpp"

namespace
{

// Работа со статическим объектом по ссылке
void InspectByRef(const baggage::BaggagePiece& piece)
{
    std::cout << "  [Осмотр по ссылке] " << piece.GetOwner() << ": " << piece.GetWeightKg() << " кг, бирка "
              << piece.GetTagNumber() << "\n";
}

// Работа с объектом по указателю
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

    std::cout << "=== Лабораторная работа 2. Классы, агрегация и композиция ===\n";
    std::cout << "Тема: система контроля и обработки багажа авиапассажиров\n";

    // --- 1. Статическая инициализация и проверка правил приёма багажа ---
    std::cout << "\n--- 1. Статические объекты: приём багажа на регистрации ---\n";
    baggage::BaggagePiece ivanov{ "Иванов И. И.", 22.0, 148.0, "7701234567" };
    baggage::BaggagePiece petrov{ "Петров П. П.", 25.5, 151.0, "7701234568" };
    baggage::BaggagePiece kuznetsov{ "Кузнецов К. К.", 15.0, 120.0, "7701234570" };
    baggage::BaggagePiece heavy{ "Сидоров С. С.", 33.5, 160.0, "7701234569" };

    std::cout << "Корректное действие: вес в норме\n";
    ivanov.Accept(23.0, false);
    std::cout << "Нарушение: превышение нормы провоза без доплаты (правило 3)\n";
    petrov.Accept(23.0, false);
    std::cout << "Исправление: сбор за сверхнорматив оплачен\n";
    petrov.Accept(23.0, true);
    std::cout << "Нарушение: вес выше предельных 32 кг даже с доплатой (правило 4)\n";
    heavy.Accept(40.0, true);
    std::cout << "Корректное действие: третье место принято, но ещё не прошло досмотр\n";
    kuznetsov.Accept(23.0, false);

    // --- 2. Композиция: место багажа владеет биркой ---
    std::cout << "\n--- 2. Композиция: обращение к части через методы целого ---\n";
    ivanov.ScanTag();
    ivanov.IssueTagStub();
    std::cout << "Повторная выдача отрывной части должна быть отклонена:\n";
    ivanov.IssueTagStub();

    // --- 3. Агрегация: рейс хранит указатели на места, созданные вне его ---
    std::cout << "\n--- 3. Агрегация: назначение мест багажа на рейс ---\n";
    baggage::Flight flight{ "SU 1602", "Барнаул", 5, 65.0 };
    flight.AddPiece(&ivanov);
    flight.AddPiece(&petrov);
    flight.AddPiece(&kuznetsov);
    std::cout << "Нарушение: превышение вместимости отсека (правило 7)\n";
    flight.AddPiece(&heavy);
    flight.PrintManifest();

    // --- 4. Досмотр, загрузка и вылет ---
    std::cout << "\n--- 4. Досмотр и загрузка багажного отсека ---\n";
    std::cout << "Нарушение: запрещённый предмет в багаже (правило 6)\n";
    petrov.PassScreening(true);
    ivanov.PassScreening(false);
    std::cout << "Снятие проблемного места с рейса (сценарий 3):\n";
    flight.RemovePiece("7701234568");
    std::cout << "Попытка загрузить место, не прошедшее досмотр (правило 5):\n";
    flight.LoadAll();
    flight.Depart();

    // --- 5. Работа с памятью ---
    std::cout << "\n--- 5. Работа с памятью ---\n";

    std::cout << "5.1. Динамический одиночный объект (new/delete):\n";
    auto* temporary = new baggage::BaggagePiece{ "Временное место", 12.0, 100.0, "7701234571" };
    temporary->Accept(23.0, false);
    delete temporary;

    std::cout << "5.2. Доступ по ссылке и по указателю:\n";
    InspectByRef(ivanov);
    InspectByPointer(&petrov);

    std::cout << "5.3. Динамический массив объектов класса (new[]/delete[]):\n";
    auto* batch = new baggage::BaggagePiece[3];
    batch[0].SetWeightKg(10.0);
    batch[0].SetSizeSumCm(115.0);
    batch[1].SetWeightKg(20.0);
    batch[1].SetSizeSumCm(135.0);
    delete[] batch;

    std::cout << "5.4. Массив динамических объектов класса:\n";
    auto** parcel = new baggage::BaggagePiece*[2];
    parcel[0] = new baggage::BaggagePiece{ "Грузинов Г. Г.", 9.0, 95.0, "7701234572" };
    std::cout << "Нарушение: неверный формат номера бирки (правило 2):\n";
    parcel[1] = new baggage::BaggagePiece{ "Ошибкин О. О.", 8.0, 90.0, "AB-12" };
    parcel[0]->Accept(23.0, false);
    std::cout << "Место без действующей бирки к перевозке не принимается:\n";
    parcel[1]->Accept(23.0, false);
    delete parcel[0];
    delete parcel[1];
    delete[] parcel;

    // --- 6. Время жизни: композиция ---
    std::cout << "\n--- 6. Композиция: уничтожение целого уничтожает часть ---\n";
    std::cout << "Вход во вложенный блок:\n";
    {
        baggage::BaggagePiece local{ "Локалов Л. Л.", 18.0, 125.0, "7701234573" };
        local.Accept(20.0, false);
        std::cout << "Выход из вложенного блока:\n";
    }
    std::cout << "Блок завершён\n";

    // --- 7. Время жизни: агрегация ---
    std::cout << "\n--- 7. Агрегация: уничтожение агрегатора сохраняет объект ---\n";
    auto* survivor = new baggage::BaggagePiece{ "Живучиков Ж. Ж.", 20.0, 130.0, "7701234574" };
    std::cout << "Вход во вложенный блок:\n";
    {
        baggage::Flight shortLived{ "SU 9999", "Томск", 3, 60.0 };
        shortLived.AddPiece(survivor);
        std::cout << "Выход из вложенного блока:\n";
    }
    std::cout << "Объект-агрегатор уничтожен; проверка, что место багажа живо:\n";
    survivor->PrintInfo();
    survivor->ScanTag();
    delete survivor;

    // --- 8. Выдача багажа в аэропорту назначения ---
    std::cout << "\n--- 8. Выдача багажа после прилёта ---\n";
    std::cout << "Нарушение: неверный номер отрывной части (правило 10)\n";
    ivanov.Deliver("1111111111");
    std::cout << "Корректное действие: номера сверены\n";
    ivanov.Deliver("7701234567");

    std::cout << "\n=== Конец демонстрации (статические объекты уничтожаются) ===\n";
    return 0;
}

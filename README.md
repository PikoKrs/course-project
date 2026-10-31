# Лабораторная работа № 3. Управляемая память и умные указатели

Выполнил: Кривов Д. Р., группа ПИ-53, вариант 98.
Тема курсового проекта: «Проектирование автоматизированной системы контроля
и обработки багажа авиапассажиров».

Работа состоит из двух частей, каждая оформлена своей веткой и отдельным
pull request'ом в `main`:

- **часть 1** (ветка `lab-03-java`) — перенос программы из ЛР 2 на Java;
- **часть 2** (эта ветка, `lab-03-cpp`) — замена обычных указателей на умные
  в проекте на C++.

## Часть 1. Java (ветка `lab-03-java`)

Те же три класса предметной области: `BaggageTag` (композиция), `BaggagePiece`,
`Flight` (агрегация), плюс `BaggageLog` — журнал оформления с `AutoCloseable`.
Код — в папке `java/src`, эталонный вывод — `java/console_output.txt`.

Сборка и запуск (JDK 17+):

    javac -encoding UTF-8 -d out java\src\baggage\*.java java\src\*.java
    java -Dfile.encoding=UTF-8 -cp out Main

Демонстрации: ссылки и копии (раздел 5), освобождение ресурса через
try-with-resources (раздел 6), сборщик мусора через `WeakReference`
(разделы 7–8).

## Часть 2. C++: умные указатели (ветка `lab-03-cpp`)

Ни одного `new` и `delete` в проекте; объекты создаются через
`std::make_unique` и `std::make_shared`. Замены по сравнению с ЛР 2:

| Где был указатель (ЛР 2)              | Заменяется на                              | Почему                                             |
|---------------------------------------|--------------------------------------------|----------------------------------------------------|
| `Flight::m_pieces` (`BaggagePiece**`) | `std::vector<std::weak_ptr<BaggagePiece>>` | агрегация: рейс не владеет, но проверяет `lock()`  |
| места багажа в `main`                 | `std::shared_ptr` + `std::make_shared`     | места должны жить, пока жив хоть один владелец     |
| `temporary` в `main` (`BaggagePiece*`) | `std::unique_ptr` + `std::make_unique`    | один владелец; передача владения через `std::move` |
| `batch` (`new BaggagePiece[3]`)       | `std::unique_ptr<BaggagePiece[]>`          | один владелец массива                              |
| `parcel` (`new BaggagePiece*[2]`)     | `std::vector<std::unique_ptr<BaggagePiece>>` | массив владельцев                                 |
| бирка в `BaggagePiece`                | без изменений (поле-объект по значению)    | композиция — самый надёжный вид владения           |

Полная таблица владения с обоснованием — в отчёте. Обычный указатель и
ссылка остались только как параметры функций, которые объектом не владеют
и нигде его не сохраняют (`InspectByRef`, `InspectByPointer`).

## Сборка и запуск части 2

MSVC (x64 Native Tools Command Prompt):

    cl /nologo /EHsc /utf-8 /std:c++17 main.cpp baggage_tag.cpp baggage_piece.cpp flight.cpp /Fe:lab03.exe
    lab03.exe

MinGW g++:

    g++ -std=c++17 -Wall main.cpp baggage_tag.cpp baggage_piece.cpp flight.cpp -o lab03
    ./lab03

Эталонный вывод части 2 — в файле `console_output.txt`.

## Отчёт

`report/Отчет_ЛР3_Кривов_Д_вариант_98.pdf` — отчёт по обеим частям:
отличия Java-версии от C++ по устройству, вывод программ с пояснениями
демонстраций, таблица владения.

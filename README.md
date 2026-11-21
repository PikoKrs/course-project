# Курсовой проект «Проектирование автоматизированной системы контроля и обработки багажа авиапассажиров»

Выполнил: Кривов Д. Р., группа ПИ-53, вариант 98.
Языки проекта: **C++** и **Java**.

Проект развивается через лабораторные работы ЛР2–ЛР5: каждая работа — следующий
этап одного и того же приложения по описанию предметной области
(приём, досмотр, сортировка, загрузка и выдача багажа авиапассажиров).

## Структура репозитория

```
cpp/    — реализация на C++ (классы предметной области)
java/   — реализация на Java (package baggage)
report/ — отчёты по работам (pdf)
```

## Сборка и запуск

### C++ (MSVC, x64 Native Tools Command Prompt)

    cl /nologo /EHsc /utf-8 /std:c++17 cpp\*.cpp /Fe:baggage.exe
    baggage.exe

Перед запуском переключите консоль в UTF-8, чтобы русский вывод читался:

    chcp 65001

### C++ (MinGW g++)

    g++ -std=c++17 -Wall cpp\*.cpp -o baggage
    ./baggage

### Java (JDK)

    javac -encoding UTF-8 -d out java/src/baggage/*.java
    java -Dfile.encoding=UTF-8 -cp out baggage.Main

## ЛР2 — классы, агрегация и композиция (ветка lab-02)

Классы (папка `cpp/`):

| Класс | Файлы | Связь | Ответственность |
|---|---|---|---|
| `BaggageTag` | `baggage_tag.hpp/.cpp` | — | багажная бирка: уникальный номер, дата оформления, сверка с отрывной частью (правило 2: номер не пуст) |
| `BaggagePiece` | `baggage_piece.hpp/.cpp` | **композиция** с биркой (поле-объект, живёт и умирает с местом) | место багажа: вес, габариты, статус; приём на стойке (правило 4: не более 32 кг), смена статуса только вперёд (правило 12) |
| `Flight` | `flight.hpp/.cpp` | **агрегация** мест (манифест хранит указатели, местами не владеет) | рейс: номер, аэропорты, вместимость; назначение мест (правило 7: мест и суммарный вес не больше вместимости отсека), закрытие регистрации (правило 9) |

Демонстрация (`cpp/main.cpp`): приём мест на рейс S7-402 Барнаул -> Москва —
корректный приём и отказы по правилам 4, 7 (вес и число мест), 9, 12;
композиция во вложенном блоке (деструкторы места и бирки подряд), агрегация
(место переживает уничтоженный рейс), работа по ссылке и по указателю,
`new`/`delete`, динамический массив объектов и массив динамических объектов.

Эталонный вывод программы — `console_output.txt`.

## ЛР3 ч.1 — перенос на Java (ветка lab-03-java)

Классы (папка `java/src/baggage/`, package `baggage`, один класс — один файл):

| Класс | Файл | Связь | Ответственность |
|---|---|---|---|
| `BaggageTag` | `BaggageTag.java` | часть места (композиция: место создаёт бирку в своём конструкторе и наружу не отдаёт) | уникальный номер, дата оформления, сверка с отрывной частью (правила 2, 10) |
| `BaggageStatus` | `BaggageStatus.java` | — | enum статусов места багажа: описание и признак проблемного статуса (правило 12) |
| `BaggagePiece` | `BaggagePiece.java` | целое для бирки | вес, габариты, статус; приём (правило 4); состояние — переопределённый `toString()` |
| `Flight` | `Flight.java` | агрегация: `List<BaggagePiece>` из созданных снаружи мест | вместимость (правило 7), закрытие регистрации (правило 9) |
| `BaggageLog` | `BaggageLog.java` | — | журнал обработки, `AutoCloseable`: освобождение ресурса через try-with-resources |
| `Main` | `Main.java` | — | демонстрация: сценарий приёма, ссылки и копии, сборщик мусора (WeakReference + System.gc()) |

Эталонный вывод Java-программы — `console_output_java.txt`.

## ЛР4 ч.2 — наследование на Java (ветка lab-04-java)

Иерархия (`java/src/baggage/`): `abstract Baggage` → `StandardBaggage`,
`OversizeBaggage`, `final FragileBaggage`; `BaggageNorm`, `EquipmentSpec`.

| Элемент | Где | Как демонстрируется |
|---|---|---|
| abstract + protected | `Baggage`: `protected final BaggageTag tag`, поля веса/статуса | наследники используют в `excessFee`/`describe` |
| `super(...)` | конструкторы наследников | вызов конструктора базы, порядок виден по выводу |
| переопределение | `StandardBaggage.describe()` через `super.describe()`; `OversizeBaggage`/`FragileBaggage` — без | дополнение vs замена |
| final | `final class FragileBaggage` | запрет наследования |
| поверхностная копия | `StandardBaggage(StandardBaggage other)` | копия делит бирку: номера совпадают, правило 2 нарушено |
| глубокая копия | `OversizeBaggage(OversizeBaggage other)` | новая бирка (генератор) и новая спецификация |
| this | `equals`, конструктор копирования | сравнение ссылок, обращение к своим полям |
| статическое поле и метод | `BaggageTag.nextNumber`, `generateNumber()` | генератор уникальных номеров бирок |
| equals/hashCode | `Baggage` | места равны, если совпадает номер бирки |
| демонстрация | `Main.java` | список `Baggage` из наследников, полиморфные сборы |

Эталонный вывод Java-программы — `console_output_java.txt`.

## ЛР5 ч.2 — полиморфизм Java (ветка lab-05-java)

| Элемент | Где | Как демонстрируется |
|---|---|---|
| интерфейсы | `Scannable` (scanBarcode), `FeePolicy` (feeFor, name) | без префикса I — соглашение Java |
| множественное наследование | `OversizeBaggage extends Baggage implements Scannable` | класс + интерфейс |
| работа через интерфейс | `SortingSystem` | `instanceof Scannable`; `FragileBaggage` — на ручную обработку |
| раннее/позднее связывание | секции 9 вывода | `final getTagNumber()` — ранняя привязка; `describe()` с `@Override` — поздняя |
| динамическое конфигурирование | `CheckInDesk` + `FeePolicy` | `setFeePolicy()`: 1500 → 750 → 2500 руб. |
| стандартный интерфейс | `Baggage implements Comparable<Baggage>` | `Collections.sort` — манифест по весу |
| исключения | `BaggageRuleException extends RuntimeException` + `OverweightException`, `CheckinClosedException`, `FlightCapacityException`, `NotClearedException`, `NotOnRouteException`, `DuplicateTagException` | проверки правил бросают |
| try/catch/finally | `Main.java` | от частного к общему; `finally`; try-with-resources с `BaggageLog` — close() при любом исходе |

Сборка: `javac -encoding UTF-8 -d out java/src/baggage/*.java`, запуск: `java -Dfile.encoding=UTF-8 -cp out baggage.Main`.
Эталонный вывод — `console_output_java.txt`.

## Предметная область

Полное описание — в отчёте по заданию 2: глоссарий, сущности и связи,
5 сценариев обработки багажа, 14 правил и ограничений, статусы места багажа.

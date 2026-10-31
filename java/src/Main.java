import baggage.BaggageLog;
import baggage.BaggagePiece;
import baggage.Flight;

import java.lang.ref.WeakReference;

/**
 * Demo program: the same scenario fragment as in LR 2 (composition,
 * aggregation, domain rule checks) plus demonstrations of how Java works
 * with memory: references instead of copies, resource release without a
 * destructor (AutoCloseable, try-with-resources) and the garbage collector.
 */
public class Main {
    public static void main(String[] args) {
        System.out.println("=== Лабораторная работа 3, часть 1. Java: язык с управляемой памятью ===");
        System.out.println("Тема: система контроля и обработки багажа авиапассажиров");

        // --- 1. Приём багажа на регистрации: та же проверка правил, что и в ЛР 2 ---
        System.out.println("\n--- 1. Приём багажа на регистрации ---");
        BaggagePiece ivanov = new BaggagePiece("Иванов И. И.", 22.0, 148.0, "7701234567");
        BaggagePiece petrov = new BaggagePiece("Петров П. П.", 25.5, 151.0, "7701234568");
        BaggagePiece kuznetsov = new BaggagePiece("Кузнецов К. К.", 15.0, 120.0, "7701234570");
        BaggagePiece heavy = new BaggagePiece("Сидоров С. С.", 33.5, 160.0, "7701234569");

        System.out.println("Корректное действие: вес в норме");
        ivanov.accept(23.0, false);
        System.out.println("Нарушение: превышение нормы провоза без доплаты (правило 3)");
        petrov.accept(23.0, false);
        System.out.println("Исправление: сбор за сверхнорматив оплачен");
        petrov.accept(23.0, true);
        System.out.println("Нарушение: вес выше предельных 32 кг даже с доплатой (правило 4)");
        heavy.accept(23.0, true);
        System.out.println("Корректное действие: третье место принято, но ещё не прошло досмотр");
        kuznetsov.accept(23.0, false);

        // --- 2. Композиция: место багажа создало свою бирку само и наружу её не отдаёт ---
        System.out.println("\n--- 2. Композиция: обращение к части через методы целого ---");
        ivanov.scanTag();
        ivanov.issueTagStub();
        System.out.println("Повторная выдача отрывной части должна быть отклонена:");
        ivanov.issueTagStub();

        // --- 3. Агрегация: рейс хранит ссылки на места, созданные вне его ---
        System.out.println("\n--- 3. Агрегация: назначение мест багажа на рейс ---");
        Flight flight = new Flight("SU 1602", "Барнаул", 5, 65.0);
        flight.addPiece(ivanov);
        flight.addPiece(petrov);
        flight.addPiece(kuznetsov);
        System.out.println("Нарушение: превышение вместимости отсека (правило 7)");
        flight.addPiece(heavy);
        flight.printManifest();

        // --- 4. Досмотр, загрузка и вылет ---
        System.out.println("\n--- 4. Досмотр и загрузка багажного отсека ---");
        System.out.println("Нарушение: запрещённый предмет в багаже (правило 6)");
        petrov.passScreening(true);
        ivanov.passScreening(false);
        System.out.println("Снятие проблемного места с рейса (сценарий 3):");
        flight.removePiece("7701234568");
        System.out.println("Попытка загрузить место, не прошедшее досмотр (правило 5):");
        flight.loadAll();
        flight.depart();

        // --- 5. Ссылки и копии: присваивание копирует ссылку, а не объект ---
        System.out.println("\n--- 5. Ссылки и копии ---");
        BaggagePiece first = new BaggagePiece("Дубов Д. Д.", 14.0, 110.0, "7701234575");
        BaggagePiece second = first; // вторая ссылка на ТОТ ЖЕ объект
        System.out.println("Изменяем вес через вторую переменную (16.5 кг):");
        second.setWeightKg(16.5);
        System.out.println("\"Первый\" объект изменился тоже: " + first.getWeightKg() + " кг");
        System.out.println("first == second: " + (first == second) + " — это один и тот же объект");

        // --- 6. Ресурс без деструктора: AutoCloseable и try-with-resources ---
        System.out.println("\n--- 6. Освобождение ресурса без деструктора ---");
        try (BaggageLog log = new BaggageLog("Оформление на рейс SU 1602")) {
            log.write("принято место багажа 7701234567 (Иванов И. И.)");
            log.write("место багажа 7701234568 снято с рейса");
        }
        System.out.println("Блок try завершён: close() вызван гарантированно при выходе из блока");

        // --- 7. Сборщик мусора: объект уничтожается не сразу ---
        System.out.println("\n--- 7. Сборщик мусора ---");
        BaggagePiece scrapped = new BaggagePiece("Списанное место", 5.0, 90.0, "7701234576");
        WeakReference<BaggagePiece> watcher = new WeakReference<>(scrapped);
        System.out.println("Ссылка есть, объект жив: " + (watcher.get() != null));
        scrapped = null; // последняя обычная ссылка исчезла
        System.out.println("Ссылок больше нет, но ничего не уничтожилось: деструктора в Java нет");
        System.gc(); // просьба собрать мусор
        System.out.println("После System.gc() объект собран: " + (watcher.get() == null));

        // --- 8. Время жизни: выход из блока не уничтожает объект ---
        // Объект создаётся в отдельном методе: после выхода из вложенного блока
        // обычных ссылок на него нет, но и в конце метода ничего не уничтожается
        System.out.println("\n--- 8. Время жизни: выход из блока ---");
        WeakReference<BaggagePiece> blockWatcher = createPieceInBlock();
        System.out.println("Блок завершён, объект ещё жив (но уже мусор): " + (blockWatcher.get() != null));
        System.gc();
        System.out.println("После System.gc() объект собран: " + (blockWatcher.get() == null));
        System.out.println("Сравните с ЛР 2: там место багажа уничтожалось ровно при выходе из блока");

        // --- 9. Агрегация: гибель агрегатора не задевает место багажа ---
        System.out.println("\n--- 9. Агрегация: уничтожение агрегатора сохраняет объект ---");
        BaggagePiece survivor = new BaggagePiece("Живучиков Ж. Ж.", 20.0, 130.0, "7701234578");
        System.out.println("Вход во вложенный блок:");
        {
            Flight shortLived = new Flight("SU 9999", "Томск", 3, 60.0);
            shortLived.addPiece(survivor);
        }
        System.out.println("Рейс больше недостижим; место багажа продолжает существовать:");
        System.out.println("  " + survivor);
        survivor.scanTag();

        // --- 10. Выдача багажа в аэропорту назначения ---
        System.out.println("\n--- 10. Выдача багажа после прилёта ---");
        System.out.println("Нарушение: неверный номер отрывной части (правило 10)");
        ivanov.deliver("1111111111");
        System.out.println("Корректное действие: номера сверены");
        ivanov.deliver("7701234567");

        System.out.println("\n=== Конец демонстрации ===");
        System.out.println("Оставшиеся объекты станут мусором и будут собраны сборщиком мусора:");
        System.out.println("момент их уничтожения заранее неизвестен, сообщений об уничтожении в Java нет");
    }

    /** Creates a piece inside a nested block; leaving the block destroys nothing in Java. */
    private static WeakReference<BaggagePiece> createPieceInBlock() {
        WeakReference<BaggagePiece> watcher;
        System.out.println("Вход во вложенный блок:");
        {
            BaggagePiece local = new BaggagePiece("Локалов Л. Л.", 18.0, 125.0, "7701234577");
            local.accept(20.0, false);
            watcher = new WeakReference<>(local);
        }
        System.out.println("Выход из вложенного блока: сообщений об уничтожении нет");
        return watcher;
    }
}

package baggage;

import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        System.out.println("=== ЛР4 ч.2. Наследование: иерархия мест багажа (Java) ===");
        System.out.println();

        System.out.println("--- 1. Норма провоза ---");
        BaggageNorm norm = new BaggageNorm();

        System.out.println();
        System.out.println("--- 2. Приёмка рейса: список Baggage разных наследников ---");
        System.out.println("Порядок конструирования: конструктор базового класса (super), затем наследника");
        List<Baggage> loadout = new ArrayList<>();
        loadout.add(new StandardBaggage("БАГ-240100", "28.10.2026", 18.0, 115.0));
        loadout.add(new StandardBaggage("БАГ-240101", "28.10.2026", 26.0, 125.0));
        loadout.add(new OversizeBaggage("БАГ-240102", "28.10.2026", 22.0, 215.0, "велосипед", 180.0));
        loadout.add(new FragileBaggage("БАГ-240103", "28.10.2026", 12.0, 105.0));

        System.out.println();
        System.out.println("Приём на стойке через ссылку на базовый класс (сбор зависит от типа места):");
        for (Baggage piece : loadout) {
            piece.acceptAtCheckIn(norm);
        }

        System.out.println();
        System.out.println("--- 3. Полиморфное describe: Standard вызывает super.describe(), остальные заменяют ---");
        for (Baggage piece : loadout) {
            System.out.println("- " + piece.describe());
        }

        System.out.println();
        System.out.println("--- 4. Поверхностная копия: копия делит бирку с оригиналом ---");
        StandardBaggage shallowOrigin = new StandardBaggage("БАГ-240115", "28.10.2026", 20.0, 100.0);
        shallowOrigin.acceptAtCheckIn(norm);
        StandardBaggage shallowCopy = new StandardBaggage(shallowOrigin);
        System.out.println("Оригинал:  " + shallowOrigin.describe());
        System.out.println("Копия:     " + shallowCopy.describe());
        System.out.println("Номера бирок совпадают: правило 2 (уникальность) нарушено поверхностной копией");
        shallowCopy.reissueDate("29.10.2026");
        System.out.println("После reissueDate у копии оригинал тоже видит новую дату:");
        System.out.println("Оригинал:  " + shallowOrigin.describe());

        System.out.println();
        System.out.println("--- 5. Глубокая копия негабарита: новая бирка и своя спецификация ---");
        OversizeBaggage oversizeOrigin = new OversizeBaggage("БАГ-240116", "28.10.2026", 21.0, 210.0, "лыжи", 170.0);
        OversizeBaggage oversizeCopy = new OversizeBaggage(oversizeOrigin);
        oversizeCopy.renameItem("сноуборд", 155.0);
        System.out.println("Оригинал:  " + oversizeOrigin.describe());
        System.out.println("Копия:     " + oversizeCopy.describe());
        System.out.println("Изменение спецификации через копию не затронуло оригинал — копия глубокая");

        System.out.println();
        System.out.println("--- 6. this и сравнение объектов по содержимому (equals по номеру бирки) ---");
        System.out.println("shallowOrigin == shallowOrigin: " + (shallowOrigin == shallowOrigin));
        System.out.println("shallowOrigin.equals(shallowCopy) по общему номеру бирки: "
                + shallowOrigin.equals(shallowCopy));
        System.out.println("shallowOrigin.equals(oversizeOrigin) по разным номерам: "
                + shallowOrigin.equals(oversizeOrigin));
        System.out.println("hashCode совпадает у равных объектов: " + (shallowOrigin.hashCode() == shallowCopy.hashCode()));
        System.out.println();

        System.out.println("--- 7. final: FragileBaggage запрещено наследовать ---");
        System.out.println("FragileBaggage объявлен final class — подклассы не допускаются");
        System.out.println();

        System.out.println("--- 8. Хрупкое место: пометка о повреждении (сценарий 5) ---");
        FragileBaggage fragile = (FragileBaggage) loadout.get(3);
        fragile.markStatus(BaggageStatus.LOADED);
        fragile.markStatus(BaggageStatus.IN_TRANSIT);
        fragile.markStatus(BaggageStatus.ISSUED);
        fragile.markDamaged();
        System.out.println(fragile.describe());

        System.out.println();
        System.out.println("=== Демонстрация завершена ===");
    }
}

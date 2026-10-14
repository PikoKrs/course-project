package baggage;

import java.lang.ref.WeakReference;

public class Main {
    public static void main(String[] args) {
        System.out.println("=== ЛР3 ч.1. Сценарий 1: приём багажа на рейс (Java) ===");
        System.out.println();

        System.out.println("--- 1. Рейс, приём мест и проверки правил (журнал try-with-resources) ---");
        try (BaggageLog log = new BaggageLog("S7-402")) {
            Flight flight = new Flight("S7-402", "Барнаул", "Москва", 2, 40.0);

            BaggagePiece first = new BaggagePiece("БАГ-240001", "13.10.2026", 23.5, 115.0);
            first.acceptAtCheckIn();
            flight.addBaggage(first);
            log.log("Принято место " + first.getTagNumber());

            BaggagePiece second = new BaggagePiece("БАГ-240002", "13.10.2026", 18.0, 125.0);
            second.acceptAtCheckIn();
            flight.addBaggage(second);

            BaggagePiece thirdLight = new BaggagePiece("БАГ-240004", "13.10.2026", 1.5, 65.0);
            thirdLight.acceptAtCheckIn();
            flight.addBaggage(thirdLight);
            log.log("Манифест заполнен: " + flight.loadedCount() + " из 2 мест");

            BaggagePiece heavy = new BaggagePiece("БАГ-240005", "13.10.2026", 35.0, 135.0);
            heavy.acceptAtCheckIn();

            BaggagePiece fourth = new BaggagePiece("БАГ-240006", "13.10.2026", 9.0, 90.0);
            fourth.acceptAtCheckIn();
            flight.addBaggage(fourth);

            flight.closeCheckIn();

            BaggagePiece late = new BaggagePiece("БАГ-240007", "13.10.2026", 12.0, 120.0);
            late.acceptAtCheckIn();
            flight.addBaggage(late);

            flight.printManifest();
            log.log("Приём завершён, регистрация закрыта");
        }
        System.out.println("Ресурс журнала освобождён автоматически при выходе из блока try-with-resources");
        System.out.println();

        System.out.println("--- 2. Ссылки и копии ---");
        BaggagePiece source = new BaggagePiece("БАГ-240020", "14.10.2026", 21.0, 110.0);
        source.acceptAtCheckIn();
        BaggagePiece samePiece = source;
        double weightCopy = source.getWeightKg();
        System.out.println("Изменяем объект через вторую ссылку:");
        samePiece.markStatus(BaggageStatus.SCREENING);
        System.out.println("Объект по первой ссылке тоже изменился: " + source);
        System.out.println("Копия примитива (double) осталась прежней: " + weightCopy + " кг");
        System.out.println();

        System.out.println("--- 3. Сборщик мусора: композиция ---");
        BaggagePiece temp = makeTempPiece();
        WeakReference<BaggagePiece> tempRef = new WeakReference<>(temp);
        System.out.println("Объект существует после создания: " + (tempRef.get() != null));
        temp = null;
        System.gc();
        System.out.println("После System.gc() место собрано сборщиком мусора: " + (tempRef.get() == null));
        System.out.println("Бирка не отдавалась наружу и ушла вместе с местом (композиция)");
        System.out.println();

        System.out.println("--- 4. Сборщик мусора: агрегация ---");
        BaggagePiece kept = new BaggagePiece("БАГ-240021", "14.10.2026", 24.0, 118.0);
        kept.acceptAtCheckIn();
        WeakReference<BaggagePiece> keptRef = new WeakReference<>(kept);
        WeakReference<Flight> localFlightRef = makeLocalFlight(kept);
        System.gc();
        System.out.println("Рейс собран сборщиком мусора: " + (localFlightRef.get() == null));
        System.out.println("Место багажа продолжает существовать: " + (keptRef.get() != null));
        System.out.println("Метод места после смерти рейса работает: " + kept);
        System.out.println();

        System.out.println("--- 5. Сверка бирки с отрывной частью при выдаче ---");
        kept.markStatus(BaggageStatus.LOADED);
        kept.markStatus(BaggageStatus.IN_TRANSIT);
        kept.markStatus(BaggageStatus.ISSUED);
        System.out.println(kept);
        kept.verifyTagAgainstStub("БАГ-240021");
        System.out.println();
        System.out.println("=== Демонстрация завершена ===");
    }

    private static BaggagePiece makeTempPiece() {
        BaggagePiece piece = new BaggagePiece("БАГ-240010", "13.10.2026", 21.0, 110.0);
        piece.acceptAtCheckIn();
        System.out.println("Ссылка на место будет обнулена после выхода из создающего метода");
        return piece;
    }

    private static WeakReference<Flight> makeLocalFlight(BaggagePiece piece) {
        Flight local = new Flight("S7-210", "Барнаул", "Новосибирск", 5, 300.0);
        local.addBaggage(piece);
        local.printManifest();
        System.out.println("Ссылка на рейс останется только в WeakReference (агрегатор переживает не место, а наоборот)");
        return new WeakReference<>(local);
    }
}

package baggage;

import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        System.out.println("=== ЛР5 ч.2. Полиморфизм: интерфейсы, конфигурирование, исключения (Java) ===");
        System.out.println();

        try (BaggageLog log = new BaggageLog("S7-402")) {
            System.out.println("--- 1. Приёмка: успех и OverweightException (правило 4) ---");
            BaggageNorm norm = new BaggageNorm();
            List<Baggage> loadout = new ArrayList<>();
            loadout.add(new StandardBaggage("БАГ-240100", "13.11.2026", 18.0, 115.0));
            loadout.add(new OversizeBaggage("БАГ-240101", "13.11.2026", 22.0, 215.0, "велосипед", 180.0));
            loadout.add(new FragileBaggage("БАГ-240102", "13.11.2026", 12.0, 105.0));

            try {
                for (Baggage piece : loadout) {
                    piece.acceptAtCheckIn(norm);
                }
                log.log("Все три места приняты без нарушений");
            } catch (OverweightException e) {
                System.out.println("  Перехвачен OverweightException (вес " + e.getWeightKg() + " кг): " + e.getMessage());
            } catch (BaggageRuleException e) {
                System.out.println("  Перехвачен BaggageRuleException: " + e.getMessage());
            }

            try {
                System.out.println("  Попытка приёма места весом 35 кг:");
                StandardBaggage heavy = new StandardBaggage("БАГ-240103", "13.11.2026", 35.0, 135.0);
                heavy.acceptAtCheckIn(norm);
            } catch (OverweightException e) {
                System.out.println("  Перехвачен OverweightException (вес " + e.getWeightKg() + " кг): " + e.getMessage());
                log.log("Отказ: перевес 35 кг");
            } catch (BaggageRuleException e) {
                System.out.println("  Перехвачен BaggageRuleException: " + e.getMessage());
            }

            System.out.println();
            System.out.println("--- 2. Раннее и позднее связывание ---");
            System.out.println("final getTagNumber() — статическая (ранняя) привязка: " + loadout.get(0).getTagNumber());
            System.out.println("describe() с @Override — динамическая (поздняя) привязка: " + loadout.get(0).describe());

            System.out.println();
            System.out.println("--- 3. Рейс: успех и CheckinClosedException (правило 9) ---");
            Flight flight = new Flight("S7-402", "Барнаул", "Москва", 2, 80.0);
            StandardBaggage checked = new StandardBaggage(BaggageTag.generateNumber(), "13.11.2026", 19.5, 110.0);
            try {
                checked.acceptAtCheckIn(norm);
                flight.addBaggage(checked);
                log.log("Место назначено на рейс S7-402");

                flight.closeCheckIn();
                System.out.println("  Попытка добавить место после закрытия регистрации:");
                flight.addBaggage(checked);
            } catch (CheckinClosedException e) {
                System.out.println("  Перехвачен CheckinClosedException (рейс " + e.getFlightNumber() + "): " + e.getMessage());
            } catch (BaggageRuleException e) {
                System.out.println("  Перехвачен BaggageRuleException: " + e.getMessage());
            }

            System.out.println();
            System.out.println("--- 4. FlightCapacityException (правило 7) ---");
            Flight capacityProbe = new Flight("S7-999", "Барнаул", "Томск", 1, 80.0);
            try {
                capacityProbe.addBaggage(new StandardBaggage(BaggageTag.generateNumber(), "13.11.2026", 15.0, 100.0));
                System.out.println("  Попытка добавить второе место в отсек на 1 место:");
                capacityProbe.addBaggage(new StandardBaggage(BaggageTag.generateNumber(), "13.11.2026", 16.0, 100.0));
            } catch (FlightCapacityException e) {
                System.out.println("  Перехвачен FlightCapacityException (рейс " + e.getFlightNumber() + "): " + e.getMessage());
            } catch (BaggageRuleException e) {
                System.out.println("  Перехвачен BaggageRuleException: " + e.getMessage());
            }

            System.out.println();
            System.out.println("--- 5. NotOnRouteException (правило 8) ---");
            try {
                flight.assign(loadout.get(0), "S7-402 / S7-210");
                System.out.println("  Попытка назначить место на рейс вне маршрута билета:");
                Flight wrongFlight = new Flight("S7-777", "Новосибирск", "Сочи", 3, 90.0);
                wrongFlight.assign(loadout.get(0), "S7-402 / S7-210");
            } catch (NotOnRouteException e) {
                System.out.println("  Перехвачен NotOnRouteException (рейс " + e.getFlightNumber() + "): " + e.getMessage());
            } catch (BaggageRuleException e) {
                System.out.println("  Перехвачен BaggageRuleException: " + e.getMessage());
            }

            System.out.println();
            System.out.println("--- 6. NotClearedException (правила 5-6) и загрузка после досмотра ---");
            try {
                flight.loadManifest();
            } catch (NotClearedException e) {
                System.out.println("  Перехвачен NotClearedException (место " + e.getTagNumber() + "): " + e.getMessage());
                log.log("Отказ загрузки: место не прошло досмотр");
            }

            System.out.println("  Место багажа " + checked.getTagNumber() + " проходит досмотр и сортировку:");
            checked.markStatus(BaggageStatus.SCREENING);
            checked.markStatus(BaggageStatus.SORTED);
            try {
                flight.loadManifest();
            } catch (NotClearedException e) {
                System.out.println("  Перехвачен NotClearedException: " + e.getMessage());
            }

            System.out.println();
            System.out.println("--- 7. DuplicateTagException (правило 2) ---");
            try {
                System.out.println("  Попытка оформить бирку с уже выданным номером БАГ-240100:");
                BaggageTag duplicate = new BaggageTag("БАГ-240100", "13.11.2026");
            } catch (DuplicateTagException e) {
                System.out.println("  Перехвачен DuplicateTagException: " + e.getMessage());
            } catch (BaggageRuleException e) {
                System.out.println("  Перехвачен BaggageRuleException: " + e.getMessage());
            } finally {
                System.out.println("  finally: обработка секции повторной бирки завершена");
            }

            System.out.println();
            System.out.println("--- 8. Общий обработчик BaggageRuleException (правило 12) ---");
            try {
                loadout.get(1).markStatus(BaggageStatus.ACCEPTED);
            } catch (BaggageRuleException e) {
                System.out.println("  Частный тип не указан — сработал общий обработчик иерархии:");
                System.out.println("  Перехвачено BaggageRuleException: " + e.getMessage());
            }

            System.out.println();
            System.out.println("--- 9. Сортировка через интерфейс Scannable (instanceof) ---");
            SortingSystem sorting = new SortingSystem();
            sorting.routeBatchToLoading(loadout);
            System.out.println("FragileBaggage не реализует Scannable — instanceof не сработал, место ушло на ручную обработку");
            log.log("Сортировка партии завершена");

            System.out.println();
            System.out.println("--- 10. Динамическое конфигурирование политики сбора ---");
            CheckInDesk desk = new CheckInDesk("N1");
            StandardBaggage feeProbe = new StandardBaggage("БАГ-240104", "13.11.2026", 26.0, 120.0);
            feeProbe.acceptAtCheckIn(norm);
            desk.registerPiece(feeProbe);
            desk.setFeePolicy(new PromoFeePolicy());
            desk.registerPiece(feeProbe);
            desk.setFeePolicy(new OversizeFeePolicy());
            desk.registerPiece(feeProbe);

            System.out.println();
            System.out.println("--- 11. Стандартный интерфейс Comparable: манифест по весу ---");
            Flight sortableFlight = new Flight("S7-402", "Барнаул", "Москва", 5, 120.0);
            sortableFlight.addBaggage(new StandardBaggage(BaggageTag.generateNumber(), "13.11.2026", 23.5, 115.0));
            sortableFlight.addBaggage(new StandardBaggage(BaggageTag.generateNumber(), "13.11.2026", 12.0, 90.0));
            sortableFlight.addBaggage(new StandardBaggage(BaggageTag.generateNumber(), "13.11.2026", 18.0, 110.0));
            System.out.println("До сортировки:");
            sortableFlight.printManifest();
            sortableFlight.sortManifestByWeight();

            System.out.println();
            System.out.println("  Исключение внутри try-with-resources: журнал закроется при любом исходе");
            String broken = null;
            try (BaggageLog closingLog = new BaggageLog("S7-210")) {
                closingLog.log("Начата обработка рейса S7-210");
                broken.length();
            } catch (NullPointerException e) {
                System.out.println("  Перехвачен NullPointerException: ресурс журнала всё равно освобождён");
            }
        }

        System.out.println();
        System.out.println("=== Демонстрация завершена ===");
    }
}

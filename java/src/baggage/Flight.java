package baggage;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

public class Flight {
    private final String number;
    private final String fromAirport;
    private final String toAirport;
    private final int capacityPlaces;
    private final double maxTotalWeightKg;
    private boolean checkInClosed;
    private final List<Baggage> manifest;

    public Flight(String number, String fromAirport, String toAirport, int capacityPlaces, double maxTotalWeightKg) {
        this.number = number;
        this.fromAirport = fromAirport;
        this.toAirport = toAirport;
        this.capacityPlaces = capacityPlaces;
        this.maxTotalWeightKg = maxTotalWeightKg;
        this.checkInClosed = false;
        this.manifest = new ArrayList<>();
        System.out.println("[Рейс] Открыт рейс " + number + " (" + fromAirport + " -> " + toAirport
                + "): вместимость " + capacityPlaces + " мест, лимит веса " + maxTotalWeightKg + " кг");
    }

    public String getNumber() {
        return number;
    }

    public int loadedCount() {
        return manifest.size();
    }

    public boolean addBaggage(Baggage piece) {
        if (checkInClosed) {
            System.out.println("[Рейс] Отказ: регистрация на рейс " + number + " закрыта, приём багажа прекращён"
                    + " (правило 9)");
            return false;
        }

        if (loadedCount() >= capacityPlaces) {
            System.out.println("[Рейс] Отказ: вместимость отсека рейса " + number + " исчерпана (" + capacityPlaces
                    + " мест), назначение места багажа " + piece.getTagNumber() + " невозможно (правило 7)");
            return false;
        }

        double newTotalWeight = loadedWeightKg() + piece.getWeightKg();
        if (newTotalWeight > maxTotalWeightKg) {
            System.out.println("[Рейс] Отказ: суммарный вес багажа рейса " + number + " (" + newTotalWeight
                    + " кг) превысил бы лимит " + maxTotalWeightKg + " кг (правило 7)");
            return false;
        }

        manifest.add(piece);
        System.out.println("[Рейс] Место багажа " + piece.getTagNumber() + " назначено на рейс " + number + " (мест: "
                + manifest.size() + " из " + capacityPlaces + ", вес: " + newTotalWeight + " из " + maxTotalWeightKg
                + " кг)");
        return true;
    }

    public boolean removeBaggage(String tagNumber) {
        Iterator<Baggage> iterator = manifest.iterator();
        while (iterator.hasNext()) {
            if (iterator.next().getTagNumber().equals(tagNumber)) {
                iterator.remove();
                System.out.println("[Рейс] Место багажа " + tagNumber + " снято с рейса " + number);
                return true;
            }
        }

        System.out.println("[Рейс] Место багажа " + tagNumber + " не найдено в манифесте рейса " + number);
        return false;
    }

    public void closeCheckIn() {
        if (checkInClosed) {
            System.out.println("[Рейс] Регистрация на рейс " + number + " уже закрыта");
            return;
        }

        checkInClosed = true;
        System.out.println("[Рейс] Регистрация на рейс " + number + " закрыта, приём багажа прекращён (правило 9)");
    }

    public void printManifest() {
        System.out.println("[Рейс] Манифест рейса " + number + " (" + fromAirport + " -> " + toAirport + "): мест "
                + manifest.size() + " из " + capacityPlaces);
        for (Baggage piece : manifest) {
            System.out.println("  " + piece);
        }
    }

    private double loadedWeightKg() {
        double total = 0.0;
        for (Baggage piece : manifest) {
            total += piece.getWeightKg();
        }
        return total;
    }

    @Override
    public String toString() {
        return "Рейс " + number + " (" + fromAirport + " -> " + toAirport + "), мест в манифесте: " + manifest.size()
                + " из " + capacityPlaces;
    }
}

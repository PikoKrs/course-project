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

    public void addBaggage(Baggage piece) {
        if (checkInClosed) {
            throw new CheckinClosedException(number);
        }

        if (loadedCount() >= capacityPlaces) {
            throw new FlightCapacityException(number,
                    "вместимость отсека исчерпана (назначено " + capacityPlaces + " мест)");
        }

        double newTotalWeight = loadedWeightKg() + piece.getWeightKg();
        if (newTotalWeight > maxTotalWeightKg) {
            throw new FlightCapacityException(number, "суммарный вес багажа " + newTotalWeight
                    + " кг превысил бы лимит " + maxTotalWeightKg + " кг");
        }

        manifest.add(piece);
        System.out.println("[Рейс] Место багажа " + piece.getTagNumber() + " назначено на рейс " + number + " (мест: "
                + manifest.size() + " из " + capacityPlaces + ", вес: " + newTotalWeight + " из " + maxTotalWeightKg
                + " кг)");
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

    public void sortManifestByWeight() {
        System.out.println("[Рейс] Сортировка манифеста рейса " + number + " по весу перед загрузкой:");
        java.util.Collections.sort(manifest);
        for (Baggage piece : manifest) {
            System.out.println("  " + piece);
        }
    }

    public void loadManifest() {
        System.out.println("[Рейс] Загрузка манифеста рейса " + number + " в багажный отсек");
        for (Baggage piece : manifest) {
            if (piece.getStatus().ordinal() < BaggageStatus.SORTED.ordinal()) {
                throw new NotClearedException(piece.getTagNumber());
            }

            piece.markStatus(BaggageStatus.LOADED);
            System.out.println("[Рейс] Место багажа " + piece.getTagNumber() + " загружено в отсек рейса " + number);
        }

        System.out.println("[Рейс] Загрузка манифеста рейса " + number + " завершена");
    }

    public void assign(Baggage piece, String ticketRoute) {
        if (!ticketRoute.contains(number)) {
            throw new NotOnRouteException(number, ticketRoute);
        }

        System.out.println("[Рейс] Место багажа " + piece.getTagNumber() + " следует по маршруту " + ticketRoute
                + ": рейс " + number + " входит в маршрут билета");
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

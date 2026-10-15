package baggage;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 * Flight: carriage between two airports with a fixed number.
 * Aggregator of baggage pieces: the flight neither creates pieces nor
 * destroys them, it only keeps references to objects created outside
 * (aggregation). In Java both composition and aggregation store references;
 * the difference is only in who creates the object.
 */
public class Flight {
    private final String number;
    private final String destination;
    private final int maxPieces;
    private final double holdCapacityKg;
    private final List<BaggagePiece> pieces = new ArrayList<>();

    public Flight(String number, String destination, int maxPieces, double holdCapacityKg) {
        this.number = number;
        this.destination = destination;
        this.maxPieces = maxPieces;
        this.holdCapacityKg = holdCapacityKg;
        System.out.println("[Flight] Открыт рейс " + number + " -> " + destination + "; отсек до "
                + formatNumber(holdCapacityKg) + " кг, не более " + maxPieces + " мест багажа");
    }

    public String getNumber() {
        return number;
    }

    public String getDestination() {
        return destination;
    }

    public int getPieceCount() {
        return pieces.size();
    }

    public double getLoadedWeightKg() {
        double total = 0.0;
        for (BaggagePiece piece : pieces) {
            total += piece.getWeightKg();
        }
        return total;
    }

    /** Assigns a piece to the flight; the hold capacity is checked (domain rule 7). */
    public boolean addPiece(BaggagePiece piece) {
        if (piece == null) {
            System.out.println("[Flight] Отказ: передана пустая ссылка на место багажа");
            return false;
        }
        if (pieces.size() >= maxPieces) {
            System.out.println("[Flight] Отказ: на рейс " + number + " уже назначено максимум мест (" + maxPieces + ")");
            return false;
        }
        double loaded = getLoadedWeightKg();
        if (loaded + piece.getWeightKg() > holdCapacityKg) {
            // Domain rule 7: the total weight must not exceed the hold capacity
            System.out.println("[Flight] Отказ: место " + piece.getTagNumber() + " (" + formatNumber(piece.getWeightKg())
                    + " кг) не помещается в отсек: занято " + formatNumber(loaded) + " из "
                    + formatNumber(holdCapacityKg) + " кг");
            return false;
        }
        pieces.add(piece);
        System.out.println("[Flight] Место багажа " + piece.getTagNumber() + " (" + piece.getOwner()
                + ") назначено на рейс " + number);
        return true;
    }

    /** Removes a piece from the flight by tag number (domain scenario 3). */
    public boolean removePiece(String tagNumber) {
        Iterator<BaggagePiece> iterator = pieces.iterator();
        while (iterator.hasNext()) {
            if (iterator.next().getTagNumber().equals(tagNumber)) {
                iterator.remove();
                System.out.println("[Flight] Место багажа " + tagNumber + " снято с рейса " + number);
                return true;
            }
        }
        System.out.println("[Flight] Отказ: место багажа " + tagNumber + " не значится на рейсе " + number);
        return false;
    }

    /** Hold loading: only screened pieces get loaded (domain rule 5). */
    public int loadAll() {
        System.out.println("[Flight] Загрузка багажного отсека рейса " + number);
        int loaded = 0;
        for (BaggagePiece piece : pieces) {
            if (piece.loadIntoCompartment()) {
                ++loaded;
            } else {
                System.out.println("[Flight] Место " + piece.getTagNumber() + " в отсек не загружено");
            }
        }
        System.out.println("[Flight] В отсеке рейса " + loaded + " из " + pieces.size() + " назначенных мест багажа");
        return loaded;
    }

    /** Departure: loaded pieces switch to the "in transit" state. */
    public void depart() {
        System.out.println("[Flight] Рейс " + number + " вылетел в " + destination);
        for (BaggagePiece piece : pieces) {
            piece.markInTransit();
        }
    }

    public void printManifest() {
        System.out.println("[Flight] Манифест рейса " + number + " -> " + destination + "; мест: " + pieces.size()
                + ", суммарный вес: " + formatNumber(getLoadedWeightKg()) + " кг");
        for (BaggagePiece piece : pieces) {
            // toString() is called implicitly when the object is concatenated with a string
            System.out.println("  " + piece);
        }
    }

    @Override
    public String toString() {
        return "Рейс " + number + " -> " + destination + "; мест багажа: " + pieces.size();
    }

    private static String formatNumber(double value) {
        return BaggagePiece.formatNumber(value);
    }
}

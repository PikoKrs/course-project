package baggage;

/**
 * Baggage tag: a unique number attached to a piece of baggage at check-in.
 * Composition part of BaggagePiece: the tag comes into being together with
 * its piece and is reclaimed by the garbage collector together with it.
 * There is no destructor in Java: once the last reference disappears,
 * the object becomes garbage and is collected at some later point.
 */
public class BaggageTag {
    /** Tag number: a ten-digit code (domain rule 2). */
    public static final int NUMBER_LENGTH = 10;

    private final String number;
    private boolean valid;
    private boolean stubIssued;

    /** Unregistered tag (placeholder number, not valid). */
    public BaggageTag() {
        this.number = "0".repeat(NUMBER_LENGTH);
    }

    /** Registers the tag, validating the number format (domain rule 2). */
    public BaggageTag(String number) {
        if (number == null || number.length() != NUMBER_LENGTH || !isDigits(number)) {
            System.out.println("[BaggageTag] Ошибка: номер \"" + number
                    + "\" должен содержать ровно 10 цифр; бирка недействительна");
            this.number = "0".repeat(NUMBER_LENGTH);
            return;
        }
        this.number = number;
        this.valid = true;
        System.out.println("[BaggageTag] Оформлена багажная бирка № " + number);
    }

    public String getNumber() {
        return number;
    }

    public boolean isValid() {
        return valid;
    }

    /** Tag reading by the airport sorting system. */
    public String scan() {
        if (!valid) {
            System.out.println("[BaggageTag] Считывание невозможно: бирка не оформлена");
            return "";
        }
        System.out.println("[BaggageTag] Сортировочная система считала бирку № " + number);
        return number;
    }

    /** Hands the tag stub to the passenger (performed exactly once). */
    public boolean issueStub() {
        if (!valid) {
            System.out.println("[BaggageTag] Отказ: нельзя выдать отрывную часть неоформленной бирки");
            return false;
        }
        if (stubIssued) {
            System.out.println("[BaggageTag] Отказ: отрывная часть бирки № " + number + " уже выдана");
            return false;
        }
        stubIssued = true;
        System.out.println("[BaggageTag] Отрывная часть бирки № " + number + " выдана пассажиру");
        return true;
    }

    @Override
    public String toString() {
        return "Багажная бирка № " + number + " (" + (valid ? "действительна" : "недействительна") + ")";
    }

    private static boolean isDigits(String text) {
        for (int i = 0; i < text.length(); i++) {
            if (!Character.isDigit(text.charAt(i))) {
                return false;
            }
        }
        return true;
    }
}

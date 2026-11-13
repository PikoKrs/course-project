package baggage;

public class BaggageTag {
    private final String number;
    private String issueDate;

    public static String generateNumber() {
        int number = nextNumber;
        nextNumber++;
        return "БАГ-" + number;
    }

    private static int nextNumber = 240110;

    public BaggageTag(String number, String issueDate) {
        if (number == null || number.isBlank()) {
            throw new DuplicateTagException("номер бирки не может быть пустым");
        }

        if (!issuedNumbers.add(number)) {
            throw new DuplicateTagException("номер бирки " + number + " уже выдан, номера бирок не повторяются");
        }

        this.number = number;
        this.issueDate = issueDate;
        System.out.println("[Бирка] Оформлена бирка " + number + " (дата " + issueDate + ")");
    }

    private static final java.util.Set<String> issuedNumbers = new java.util.HashSet<>();

    public String getNumber() {
        return number;
    }

    public String getIssueDate() {
        return issueDate;
    }

    public void changeIssueDate(String issueDate) {
        this.issueDate = issueDate;
        System.out.println("[Бирка] Бирка " + number + " переоформлена на дату " + this.issueDate);
    }

    public boolean verifyAgainstStub(String stubNumber) {
        if (number.equals(stubNumber)) {
            System.out.println("[Бирка] Номер " + number + " совпадает с отрывной частью: место выдано");
            return true;
        }
        System.out.println("[Бирка] Отказ: номер " + number + " не совпадает с отрывной частью " + stubNumber
                + " (правило 10)");
        return false;
    }

    @Override
    public String toString() {
        return "Бирка " + number + ", дата оформления " + issueDate;
    }
}

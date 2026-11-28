package baggage;

public class BaggageTag {
    private final String number;
    private final String issueDate;

    public BaggageTag(String number, String issueDate) {
        if (number == null || number.isBlank()) {
            System.out.println("[Бирка] Отказ: номер бирки не может быть пустым (правило 2)");
            this.number = "000000";
            this.issueDate = "-";
            System.out.println("[Бирка] Выдана временная бирка 000000 (номер не присвоен)");
        } else {
            this.number = number;
            this.issueDate = issueDate;
            System.out.println("[Бирка] Оформлена бирка " + number + " (дата " + issueDate + ")");
        }
    }

    public String getNumber() {
        return number;
    }

    public String getIssueDate() {
        return issueDate;
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

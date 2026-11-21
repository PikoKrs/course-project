package baggage;

public class NotClearedException extends BaggageRuleException {
    private final String tagNumber;

    public NotClearedException(String tagNumber) {
        super("правила 5-6: место багажа " + tagNumber + " не прошло досмотр, загрузка в рейс запрещена");
        this.tagNumber = tagNumber;
    }

    public String getTagNumber() {
        return tagNumber;
    }
}

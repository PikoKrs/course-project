package baggage;

public class DuplicateTagException extends BaggageRuleException {
    public DuplicateTagException(String reason) {
        super("правило 2: " + reason);
    }
}

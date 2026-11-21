package baggage;

public class CheckinClosedException extends BaggageRuleException {
    private final String flightNumber;

    public CheckinClosedException(String flightNumber) {
        super("правило 9: регистрация на рейс " + flightNumber + " закрыта, приём багажа прекращён");
        this.flightNumber = flightNumber;
    }

    public String getFlightNumber() {
        return flightNumber;
    }
}

package baggage;

public class FlightCapacityException extends BaggageRuleException {
    private final String flightNumber;

    public FlightCapacityException(String flightNumber, String reason) {
        super("правило 7: рейс " + flightNumber + " не принимает место багажа: " + reason);
        this.flightNumber = flightNumber;
    }

    public String getFlightNumber() {
        return flightNumber;
    }
}

package baggage;

public class NotOnRouteException extends BaggageRuleException {
    private final String flightNumber;

    public NotOnRouteException(String flightNumber, String ticketRoute) {
        super("правило 8: рейс " + flightNumber + " не входит в маршрут билета " + ticketRoute);
        this.flightNumber = flightNumber;
    }

    public String getFlightNumber() {
        return flightNumber;
    }
}

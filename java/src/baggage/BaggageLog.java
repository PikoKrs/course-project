package baggage;

public class BaggageLog implements AutoCloseable {
    private final String flightNumber;
    private int entries;

    public BaggageLog(String flightNumber) {
        this.flightNumber = flightNumber;
        System.out.println("[Журнал] Открыт журнал обработки рейса " + flightNumber);
    }

    public void log(String message) {
        entries++;
        System.out.println("[Журнал " + entries + "] " + message);
    }

    @Override
    public void close() {
        System.out.println("[Журнал] Журнал рейса " + flightNumber + " закрыт, записей: " + entries
                + ", ресурс освобождён");
    }
}

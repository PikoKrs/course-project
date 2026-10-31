package baggage;

/**
 * Baggage handling log: an auxiliary class demonstrating how a resource is
 * released in a language without destructors. Java cannot promise when (or
 * whether) the object itself is reclaimed, so everything that must happen
 * in time is done explicitly: the class implements AutoCloseable and the
 * try-with-resources block guarantees the close() call.
 */
public class BaggageLog implements AutoCloseable {
    private final String title;

    public BaggageLog(String title) {
        this.title = title;
        System.out.println("[+] журнал открыт: " + title);
    }

    public void write(String line) {
        System.out.println("    " + line);
    }

    @Override
    public void close() {
        System.out.println("[-] журнал закрыт: " + title);
    }

    @Override
    public String toString() {
        return "Журнал \"" + title + "\"";
    }
}

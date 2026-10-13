package baggage;

public enum BaggageStatus {
    NONE("не оформлено"),
    ACCEPTED("принято"),
    SCREENING("на досмотре"),
    SORTED("отсортировано"),
    LOADED("загружено"),
    IN_TRANSIT("в пути"),
    ISSUED("выдано"),
    DELAYED("задержано"),
    LOST("утеряно"),
    REMOVED("снято с рейса");

    private final String description;

    BaggageStatus(String description) {
        this.description = description;
    }

    public String description() {
        return description;
    }

    public boolean isProblem() {
        return this == DELAYED || this == LOST || this == REMOVED;
    }
}

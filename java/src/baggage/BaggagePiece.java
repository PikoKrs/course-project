package baggage;

/**
 * Piece of baggage: a physical unit of checked baggage (a suitcase, a bag,
 * a box). Owns its baggage tag as a composition part: the piece creates the
 * tag itself in its constructor and never hands the tag out. At the same
 * time the piece exists independently of a flight: the flight only keeps a
 * reference to it (aggregation).
 */
public class BaggagePiece {
    /**
     * Processing chain states. The declaration order matters: the status may
     * only move forward along the chain (domain rule 12).
     */
    public enum Status {
        NEW("не оформлено"),
        ACCEPTED("принято"),
        SCREENED("прошло досмотр"),
        LOADED("загружено"),
        IN_TRANSIT("в пути"),
        DELIVERED("выдано"),
        OFFLOADED("снято с рейса");

        private final String description;

        Status(String description) {
            this.description = description;
        }

        public String getDescription() {
            return description;
        }
    }

    /** Hard weight limit of one piece: impossible even with a fee (domain rule 4). */
    public static final double MAX_WEIGHT_KG = 32.0;

    private final String owner;
    private double weightKg;
    private double sizeSumCm;
    private final BaggageTag tag; // composition: the tag is an integral part of the piece
    private Status status;

    public BaggagePiece(String owner, double weightKg, double sizeSumCm, String tagNumber) {
        this.owner = owner;
        this.weightKg = weightKg;
        this.sizeSumCm = sizeSumCm;
        // Composition: the whole creates its part itself, in its own constructor
        this.tag = new BaggageTag(tagNumber);
        this.status = Status.NEW;
        System.out.println("[BaggagePiece] Создано место багажа: " + owner + ", " + formatNumber(weightKg) + " кг");
    }

    /** Unregistered piece (placeholder owner and tag). */
    public BaggagePiece() {
        this.owner = "без владельца";
        this.tag = new BaggageTag();
        this.status = Status.NEW;
        System.out.println("[BaggagePiece] Создано неоформленное место багажа");
    }

    public String getOwner() {
        return owner;
    }

    public double getWeightKg() {
        return weightKg;
    }

    public double getSizeSumCm() {
        return sizeSumCm;
    }

    /** Only the tag number is exposed; the tag itself never leaves the piece. */
    public String getTagNumber() {
        return tag.getNumber();
    }

    public Status getStatus() {
        return status;
    }

    public void setWeightKg(double weightKg) {
        if (weightKg < 0.0) {
            System.out.println("[BaggagePiece] Отказ: вес места багажа не может быть отрицательным");
            return;
        }
        this.weightKg = weightKg;
    }

    public void setSizeSumCm(double sizeSumCm) {
        if (sizeSumCm < 0.0) {
            System.out.println("[BaggagePiece] Отказ: габариты места багажа не могут быть отрицательными");
            return;
        }
        this.sizeSumCm = sizeSumCm;
    }

    /**
     * Acceptance at check-in: normKg is the ticket baggage allowance,
     * feePaid tells whether the oversize fee has been paid (domain rules 2-4).
     */
    public boolean accept(double normKg, boolean feePaid) {
        if (status != Status.NEW) {
            System.out.println("[BaggagePiece] Отказ: место багажа уже оформлено");
            return false;
        }
        if (!tag.isValid()) {
            // Domain rule 2: a piece without a valid tag is never accepted
            System.out.println("[BaggagePiece] Отказ: место багажа не имеет действующей бирки");
            return false;
        }
        if (weightKg > MAX_WEIGHT_KG) {
            // Domain rule 4: no single piece may exceed 32 kg
            System.out.println("[BaggagePiece] Отказ: вес " + formatNumber(weightKg) + " кг превышает предельные "
                    + formatNumber(MAX_WEIGHT_KG) + " кг; оформление возможно только по грузовым правилам");
            return false;
        }
        if (weightKg > normKg && !feePaid) {
            // Domain rule 3: over the allowance without the paid fee
            System.out.println("[BaggagePiece] Отказ: вес " + formatNumber(weightKg) + " кг превышает норму провоза "
                    + formatNumber(normKg) + " кг; требуется оплата сбора за сверхнормативный багаж");
            return false;
        }
        if (weightKg > normKg) {
            System.out.println("[BaggagePiece] Сбор за сверхнормативный багаж оплачен; место принято с пометкой");
        } else {
            System.out.println("[BaggagePiece] Место багажа укладывается в норму провоза " + formatNumber(normKg) + " кг");
        }
        return changeStatus(Status.ACCEPTED, "приём на рейс");
    }

    /** Screening: a prohibited item takes the piece off the flight (domain rule 6). */
    public boolean passScreening(boolean prohibitedFound) {
        if (status != Status.ACCEPTED) {
            System.out.println("[BaggagePiece] Отказ: на досмотр направляется только место со статусом \"принято\"");
            return false;
        }
        if (prohibitedFound) {
            System.out.println("[BaggagePiece] Обнаружен запрещённый предмет; место снимается с рейса");
            return changeStatus(Status.OFFLOADED, "досмотр");
        }
        System.out.println("[BaggagePiece] Досмотр пройден: запрещённых предметов нет");
        return changeStatus(Status.SCREENED, "досмотр");
    }

    /** Loading into the hold; only allowed after screening (domain rule 5). */
    public boolean loadIntoCompartment() {
        if (status != Status.SCREENED) {
            System.out.println("[BaggagePiece] Отказ: место не прошло досмотр и не может быть загружено в рейс");
            return false;
        }
        return changeStatus(Status.LOADED, "загрузка в отсек");
    }

    /** Departure: the piece switches to the "in transit" state. */
    public boolean markInTransit() {
        if (status != Status.LOADED) {
            System.out.println("[BaggagePiece] Отказ: в статус \"в пути\" переходит только загруженное место багажа");
            return false;
        }
        return changeStatus(Status.IN_TRANSIT, "вылет рейса");
    }

    /** Delivery after comparing the stub number with the tag number (domain rule 10). */
    public boolean deliver(String stubNumber) {
        if (status != Status.IN_TRANSIT) {
            System.out.println("[BaggagePiece] Отказ: выдача возможна только после прилёта рейса (статус \"в пути\")");
            return false;
        }
        if (!stubNumber.equals(tag.getNumber())) {
            System.out.println("[BaggagePiece] Отказ: номер отрывной части \"" + stubNumber
                    + "\" не совпадает с номером бирки \"" + tag.getNumber() + "\"");
            return false;
        }
        System.out.println("[BaggagePiece] Номер отрывной части сверен с номером бирки");
        return changeStatus(Status.DELIVERED, "выдача");
    }

    /** Composition in action: the whole works with its part via its own method. */
    public String scanTag() {
        return tag.scan();
    }

    /** Composition in action: handing the tag stub to the passenger. */
    public boolean issueTagStub() {
        return tag.issueStub();
    }

    @Override
    public String toString() {
        return "Место багажа [" + owner + "] вес: " + formatNumber(weightKg) + " кг, сумма габаритов: "
                + formatNumber(sizeSumCm) + " см, бирка: " + tag.getNumber()
                + ", статус: " + status.getDescription();
    }

    // Domain rule 12: the status never moves backwards along the chain
    private boolean changeStatus(Status next, String action) {
        if (next.ordinal() <= status.ordinal()) {
            System.out.println("[BaggagePiece] Отказ (" + action + "): статус \"" + status.getDescription()
                    + "\" не может измениться на \"" + next.getDescription() + "\" задним числом");
            return false;
        }
        status = next;
        System.out.println("[BaggagePiece] " + owner + ": статус \"" + status.getDescription() + "\"");
        return true;
    }

    /** Prints 22 instead of 22.0 and keeps 25.5 as is (like the C++ streams in LR 2). */
    static String formatNumber(double value) {
        if (value == Math.rint(value) && !Double.isInfinite(value)) {
            return String.valueOf((long) value);
        }
        return String.valueOf(value);
    }
}

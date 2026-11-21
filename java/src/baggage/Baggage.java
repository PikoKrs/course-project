package baggage;

public abstract class Baggage implements Comparable<Baggage> {
    protected final BaggageTag tag;
    protected double weightKg;
    protected double dimsSumCm;
    protected BaggageStatus status;

    protected Baggage(String issueDate, double weightKg, double dimsSumCm) {
        this(BaggageTag.generateNumber(), issueDate, weightKg, dimsSumCm);
        System.out.println("[Базовый] Номер бирки выдан статическим генератором BaggageTag.generateNumber");
    }

    protected Baggage(String tagNumber, String issueDate, double weightKg, double dimsSumCm) {
        this.tag = new BaggageTag(tagNumber, issueDate);
        this.weightKg = weightKg;
        this.dimsSumCm = dimsSumCm;
        this.status = BaggageStatus.NONE;
        System.out.println("[Базовый] Сформировано место багажа: вес " + weightKg + " кг, габариты " + dimsSumCm
                + " см");
    }

    public final String getTagNumber() {
        return tag.getNumber();
    }

    public double getWeightKg() {
        return weightKg;
    }

    public BaggageStatus getStatus() {
        return status;
    }

    public void acceptAtCheckIn(BaggageNorm norm) {
        if (weightKg > norm.getAbsoluteMaxWeightKg()) {
            throw new OverweightException(weightKg);
        }

        if (status != BaggageStatus.NONE) {
            throw new BaggageRuleException("место багажа " + getTagNumber() + " уже в обработке (статус: "
                    + status.description() + "), повторный приём недопустим");
        }

        status = BaggageStatus.ACCEPTED;
        double fee = excessFee(norm);
        if (fee > 0.0) {
            System.out.println("[Место] Место багажа " + getTagNumber() + " принято со сверхнормативом, сбор: " + fee
                    + " руб.");
        } else {
            System.out.println("[Место] Место багажа " + getTagNumber() + " принято, укладывается в норму тарифа");
        }
    }

    public void markStatus(BaggageStatus newStatus) {
        if (newStatus == status) {
            throw new BaggageRuleException("правило 12: статус места багажа " + getTagNumber() + " уже "
                    + status.description());
        }

        if (status == BaggageStatus.NONE && newStatus != BaggageStatus.ACCEPTED) {
            throw new BaggageRuleException("правило 12: место багажа " + getTagNumber()
                    + " ещё не принято, первый статус должен быть \"принято\"");
        }

        if (!newStatus.isProblem() && !status.isProblem() && newStatus != BaggageStatus.NONE
                && newStatus.ordinal() < status.ordinal()) {
            throw new BaggageRuleException("правило 12: статус места багажа не меняется в обратную сторону: "
                    + status.description() + " -> " + newStatus.description());
        }

        status = newStatus;
        System.out.println("[Место] Бирка " + getTagNumber() + ": новый статус: " + status.description());
    }

    public void reissueDate(String issueDate) {
        tag.changeIssueDate(issueDate);
    }

    public abstract double excessFee(BaggageNorm norm);

    public String describe() {
        return "Место багажа " + getTagNumber() + ": вес " + weightKg + " кг, габариты " + dimsSumCm + " см, статус: "
                + status.description();
    }

    @Override
    public String toString() {
        return "Место багажа " + getTagNumber() + ": вес " + weightKg + " кг, габариты " + dimsSumCm + " см, статус: "
                + status.description();
    }

    @Override
    public int compareTo(Baggage other) {
        return Double.compare(weightKg, other.weightKg);
    }

    @Override
    public boolean equals(Object other) {
        if (this == other) {
            return true;
        }

        if (!(other instanceof Baggage)) {
            return false;
        }

        Baggage baggage = (Baggage) other;
        return getTagNumber().equals(baggage.getTagNumber());
    }

    @Override
    public int hashCode() {
        return getTagNumber().hashCode();
    }
}

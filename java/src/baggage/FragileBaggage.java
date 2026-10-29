package baggage;

public class FragileBaggage extends Baggage {    private boolean damageMarked;

    public FragileBaggage(String tagNumber, String issueDate, double weightKg, double dimsSumCm) {
        super(tagNumber, issueDate, weightKg, dimsSumCm);
        this.damageMarked = false;
        System.out.println("[Хрупкое] Оформлено хрупкое место багажа (акты о повреждении фиксируются по сценарию 5)");
    }

    public void markDamaged() {
        damageMarked = true;
        System.out.println("[Хрупкое] К месту багажа " + getTagNumber() + " добавлена пометка \"повреждено\"");
    }

    @Override
    public double excessFee(BaggageNorm norm) {
        return 0.0;
    }

    @Override
    public String describe() {
        if (damageMarked) {
            return "Хрупкое место багажа " + getTagNumber() + ": вес " + weightKg
                    + " кг, статус: выдано с пометкой \"повреждено\" (сценарий 5)";
        }

        return "Хрупкое место багажа " + getTagNumber() + ": вес " + weightKg + " кг, повреждений нет";
    }
}

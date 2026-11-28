package baggage;

public class OversizeBaggage extends Baggage {
    public static final double FIXED_FEE = 2500.0;

    protected final EquipmentSpec spec;

    public OversizeBaggage(String tagNumber, String issueDate, double weightKg, double dimsSumCm, String itemName,
                           double itemLengthCm) {
        super(tagNumber, issueDate, weightKg, dimsSumCm);
        this.spec = new EquipmentSpec(itemName, itemLengthCm);
        System.out.println("[Негабарит] Оформлено негабаритное место багажа: " + spec + ", фиксированный сбор "
                + FIXED_FEE + " руб.");
    }

    public OversizeBaggage(OversizeBaggage other) {
        super(BaggageTag.generateNumber(), other.tag.getIssueDate(), other.weightKg, other.dimsSumCm);
        this.spec = new EquipmentSpec(other.spec.getItemName(), other.spec.getItemLengthCm());
        this.status = other.status;
        System.out.println("[Негабарит] Глубокая копия: собственная бирка " + getTagNumber()
                + " и собственная спецификация");
    }

    public void renameItem(String itemName, double itemLengthCm) {
        spec.changeItem(itemName, itemLengthCm);
    }

    @Override
    public double excessFee(BaggageNorm norm) {
        return FIXED_FEE;
    }

    @Override
    public String describe() {
        return "Негабаритное место багажа " + getTagNumber() + ": предмет " + spec + ", вес " + weightKg
                + " кг, габариты " + dimsSumCm + " см, фиксированный сбор";
    }
}

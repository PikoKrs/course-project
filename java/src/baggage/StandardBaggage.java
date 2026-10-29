package baggage;

public class StandardBaggage extends Baggage {
    public static final double FEE_PER_KG = 500.0;

    public StandardBaggage(String tagNumber, String issueDate, double weightKg, double dimsSumCm) {
        super(tagNumber, issueDate, weightKg, dimsSumCm);
        System.out.println("[Стандарт] Обычное место багажа по тарифу, сбор за перевес: " + FEE_PER_KG
                + " руб. за кг");
    }

    public StandardBaggage(StandardBaggage other) {
        super(other.tag.getNumber(), other.tag.getIssueDate(), other.weightKg, other.dimsSumCm);
        this.status = other.status;
        System.out.println("[Стандарт] Поверхностная копия: бирка общая с оригиналом (номер " + getTagNumber() + ")");
    }

    @Override
    public double excessFee(BaggageNorm norm) {
        if (weightKg <= norm.getMaxWeightKg()) {
            return 0.0;
        }

        return (weightKg - norm.getMaxWeightKg()) * FEE_PER_KG;
    }

    @Override
    public String describe() {
        return super.describe() + "; обычное место багажа, действует норма тарифа";
    }
}

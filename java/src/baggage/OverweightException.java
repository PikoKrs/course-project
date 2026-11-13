package baggage;

public class OverweightException extends BaggageRuleException {
    private final double weightKg;

    public OverweightException(double weightKg) {
        super("правило 4: вес места багажа " + weightKg + " кг превышает предельные 32 кг, приём невозможен");
        this.weightKg = weightKg;
    }

    public double getWeightKg() {
        return weightKg;
    }
}

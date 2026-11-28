package baggage;

public class BaggageNorm {
    private final double maxWeightKg;
    private final double maxDimsSumCm;
    private final double absoluteMaxWeightKg;

    public BaggageNorm() {
        this(23.0, 158.0, 32.0);
        System.out.println("[Норма] Принята типовая норма провоза: " + this);
    }

    public BaggageNorm(double maxWeightKg, double maxDimsSumCm, double absoluteMaxWeightKg) {
        this.maxWeightKg = maxWeightKg;
        this.maxDimsSumCm = maxDimsSumCm;
        this.absoluteMaxWeightKg = absoluteMaxWeightKg;
    }

    public double getMaxWeightKg() {
        return maxWeightKg;
    }

    public double getMaxDimsSumCm() {
        return maxDimsSumCm;
    }

    public double getAbsoluteMaxWeightKg() {
        return absoluteMaxWeightKg;
    }

    @Override
    public String toString() {
        return "норма: " + maxWeightKg + " кг на место, " + maxDimsSumCm + " см габаритов, предельный вес "
                + absoluteMaxWeightKg + " кг";
    }
}

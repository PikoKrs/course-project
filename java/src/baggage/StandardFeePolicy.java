package baggage;

public class StandardFeePolicy implements FeePolicy {
    @Override
    public double feeFor(Baggage piece, BaggageNorm norm) {
        if (piece.getWeightKg() <= norm.getMaxWeightKg()) {
            return 0.0;
        }

        return (piece.getWeightKg() - norm.getMaxWeightKg()) * 500.0;
    }

    @Override
    public String name() {
        return "базовая: 500 руб. за каждый кг перевеса";
    }
}

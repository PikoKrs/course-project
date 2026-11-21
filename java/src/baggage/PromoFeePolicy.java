package baggage;

public class PromoFeePolicy implements FeePolicy {
    @Override
    public double feeFor(Baggage piece, BaggageNorm norm) {
        if (piece.getWeightKg() <= norm.getMaxWeightKg()) {
            return 0.0;
        }

        return (piece.getWeightKg() - norm.getMaxWeightKg()) * 250.0;
    }

    @Override
    public String name() {
        return "акция: 250 руб. за каждый кг перевеса (скидка 50%)";
    }
}

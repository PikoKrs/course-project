package baggage;

public class OversizeFeePolicy implements FeePolicy {
    @Override
    public double feeFor(Baggage piece, BaggageNorm norm) {
        return 2500.0;
    }

    @Override
    public String name() {
        return "негабарит: фиксированный сбор 2500 руб.";
    }
}

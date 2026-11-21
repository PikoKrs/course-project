package baggage;

public interface FeePolicy {
    double feeFor(Baggage piece, BaggageNorm norm);

    String name();
}

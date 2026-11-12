package baggage;

public class CheckInDesk {
    private final String deskName;
    private FeePolicy feePolicy;
    private final BaggageNorm norm = new BaggageNorm();

    public CheckInDesk(String deskName) {
        this.deskName = deskName;
        this.feePolicy = new StandardFeePolicy();
        System.out.println("[Стойка] Открыта стойка регистрации " + deskName + ", политика сбора: " + feePolicy.name());
    }

    public void setFeePolicy(FeePolicy policy) {
        System.out.println("[Стойка] Стойка " + deskName + ": политика сбора сменилась \"" + feePolicy.name()
                + "\" -> \"" + policy.name() + "\"");
        this.feePolicy = policy;
    }

    public void registerPiece(Baggage piece) {
        double fee = feePolicy.feeFor(piece, norm);
        System.out.println("[Стойка] " + deskName + ": место багажа " + piece.getTagNumber() + " ("
                + piece.getWeightKg() + " кг), сбор по политике \"" + feePolicy.name() + "\": " + fee + " руб.");
    }
}

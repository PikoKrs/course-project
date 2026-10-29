package baggage;

public class EquipmentSpec {
    private String itemName;
    private double itemLengthCm;

    public EquipmentSpec(String itemName, double itemLengthCm) {
        this.itemName = itemName;
        this.itemLengthCm = itemLengthCm;
    }

    public String getItemName() {
        return itemName;
    }

    public double getItemLengthCm() {
        return itemLengthCm;
    }

    public void changeItem(String itemName, double itemLengthCm) {
        this.itemName = itemName;
        this.itemLengthCm = itemLengthCm;
    }

    @Override
    public String toString() {
        return itemName + " (" + itemLengthCm + " см)";
    }
}

package baggage;

public class SortingSystem {
    public void routeToLoading(Baggage piece) {
        if (!(piece instanceof Scannable)) {
            System.out.println("[Сортировка] Место багажа " + piece.getTagNumber()
                    + " не поддерживает Scannable: направляется на ручную обработку");
            return;
        }

        if (piece.getStatus().ordinal() >= BaggageStatus.SORTED.ordinal()) {
            System.out.println("[Сортировка] Место багажа " + piece.getTagNumber()
                    + " уже прошло сортировку, повторное направление не требуется");
            return;
        }

        Scannable scannable = (Scannable) piece;
        String barcode = scannable.scanBarcode();
        piece.markStatus(BaggageStatus.SORTED);
        System.out.println("[Сортировка] Место багажа " + barcode + " направлено к загрузке, статус: отсортировано");
    }

    public void routeBatchToLoading(java.util.List<Baggage> pieces) {
        System.out.println("[Сортировка] Начата обработка партии из " + pieces.size() + " мест багажа");
        for (Baggage piece : pieces) {
            routeToLoading(piece);
        }
    }
}

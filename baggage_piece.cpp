//
// ФАЙЛ baggage_piece.cpp
//
#include "baggage_piece.hpp"

#include <iostream>

namespace baggage
{

std::string_view BaggagePiece::StatusToString(Status status)
{
    switch (status)
    {
        case Status::eNew:
            return "не оформлено";
        case Status::eAccepted:
            return "принято";
        case Status::eScreened:
            return "прошло досмотр";
        case Status::eLoaded:
            return "загружено";
        case Status::eInTransit:
            return "в пути";
        case Status::eDelivered:
            return "выдано";
        case Status::eOffloaded:
            return "снято с рейса";
    }
    return "неизвестно";
}

BaggagePiece::BaggagePiece(std::string_view owner, double weightKg, double sizeSumCm, std::string_view tagNumber)
    : m_owner{ std::string(owner) }
    , m_weightKg{ weightKg }
    , m_sizeSumCm{ sizeSumCm }
    , m_tag{ tagNumber }
{
    std::cout << "[BaggagePiece] Создано место багажа: " << m_owner << ", " << m_weightKg << " кг\n";
}

BaggagePiece::BaggagePiece()
    : m_owner{ "без владельца" }
{
    std::cout << "[BaggagePiece] Создано неоформленное место багажа\n";
}

BaggagePiece::~BaggagePiece()
{
    // Композиция: вместе с местом багажа автоматически уничтожается его бирка
    std::cout << "[~BaggagePiece] Место багажа (" << m_owner << ") уничтожено вместе со своей биркой\n";
}

void BaggagePiece::SetWeightKg(double weightKg)
{
    if (weightKg < 0.0)
    {
        std::cout << "[BaggagePiece] Отказ: вес места багажа не может быть отрицательным\n";
        return;
    }
    m_weightKg = weightKg;
}

void BaggagePiece::SetSizeSumCm(double sizeSumCm)
{
    if (sizeSumCm < 0.0)
    {
        std::cout << "[BaggagePiece] Отказ: габариты места багажа не могут быть отрицательными\n";
        return;
    }
    m_sizeSumCm = sizeSumCm;
}

bool BaggagePiece::Accept(double normKg, bool feePaid)
{
    if (m_status != Status::eNew)
    {
        std::cout << "[BaggagePiece] Отказ: место багажа уже оформлено\n";
        return false;
    }
    if (!m_tag.IsValid())
    {
        // Правило 2: место без действующей бирки не принимается и не перевозится
        std::cout << "[BaggagePiece] Отказ: место багажа не имеет действующей бирки\n";
        return false;
    }
    if (m_weightKg > kMaxWeightKg)
    {
        // Правило 4: вес одного места не может превышать 32 кг ни при какой доплате
        std::cout << "[BaggagePiece] Отказ: вес " << m_weightKg << " кг превышает предельные " << kMaxWeightKg
                  << " кг; оформление возможно только по грузовым правилам\n";
        return false;
    }
    if (m_weightKg > normKg && !feePaid)
    {
        // Правило 3: превышение нормы провоза без оформленной доплаты
        std::cout << "[BaggagePiece] Отказ: вес " << m_weightKg << " кг превышает норму провоза " << normKg
                  << " кг; требуется оплата сбора за сверхнормативный багаж\n";
        return false;
    }
    if (m_weightKg > normKg)
    {
        std::cout << "[BaggagePiece] Сбор за сверхнормативный багаж оплачен; место принято с пометкой\n";
    }
    else
    {
        std::cout << "[BaggagePiece] Место багажа укладывается в норму провоза " << normKg << " кг\n";
    }
    return ChangeStatus(Status::eAccepted, "приём на рейс");
}

bool BaggagePiece::PassScreening(bool prohibitedFound)
{
    if (m_status != Status::eAccepted)
    {
        std::cout << "[BaggagePiece] Отказ: на досмотр направляется только место со статусом \"принято\"\n";
        return false;
    }
    if (prohibitedFound)
    {
        // Правило 6: место с выявленным нарушением не может быть загружено в рейс
        std::cout << "[BaggagePiece] Обнаружен запрещённый предмет; место снимается с рейса\n";
        return ChangeStatus(Status::eOffloaded, "досмотр");
    }
    std::cout << "[BaggagePiece] Досмотр пройден: запрещённых предметов нет\n";
    return ChangeStatus(Status::eScreened, "досмотр");
}

bool BaggagePiece::LoadIntoCompartment()
{
    if (m_status != Status::eScreened)
    {
        // Правило 5: до успешного прохождения досмотра загрузка в рейс невозможна
        std::cout << "[BaggagePiece] Отказ: место не прошло досмотр и не может быть загружено в рейс\n";
        return false;
    }
    return ChangeStatus(Status::eLoaded, "загрузка в отсек");
}

bool BaggagePiece::MarkInTransit()
{
    if (m_status != Status::eLoaded)
    {
        std::cout << "[BaggagePiece] Отказ: в статус \"в пути\" переходит только загруженное место багажа\n";
        return false;
    }
    return ChangeStatus(Status::eInTransit, "вылет рейса");
}

bool BaggagePiece::Deliver(std::string_view stubNumber)
{
    if (m_status != Status::eInTransit)
    {
        std::cout << "[BaggagePiece] Отказ: выдача возможна только после прилёта рейса (статус \"в пути\")\n";
        return false;
    }
    if (stubNumber != m_tag.GetNumber())
    {
        // Правило 10: номер отрывной части не сверен с номером бирки — выдача невозможна
        std::cout << "[BaggagePiece] Отказ: номер отрывной части \"" << stubNumber
                  << "\" не совпадает с номером бирки \"" << m_tag.GetNumber() << "\"\n";
        return false;
    }
    std::cout << "[BaggagePiece] Номер отрывной части сверен с номером бирки\n";
    return ChangeStatus(Status::eDelivered, "выдача");
}

std::string BaggagePiece::ScanTag() const
{
    // Композиция: целое работает со своей частью через собственный метод
    return m_tag.Scan();
}

bool BaggagePiece::IssueTagStub()
{
    return m_tag.IssueStub();
}

bool BaggagePiece::ChangeStatus(Status next, std::string_view action)
{
    if (next <= m_status)
    {
        // Правило 12: статус не меняется в обратную сторону по цепочке обработки
        std::cout << "[BaggagePiece] Отказ (" << action << "): статус \"" << StatusToString(m_status)
                  << "\" не может измениться на \"" << StatusToString(next) << "\" задним числом\n";
        return false;
    }
    m_status = next;
    std::cout << "[BaggagePiece] " << m_owner << ": статус \"" << StatusToString(m_status) << "\"\n";
    return true;
}

void BaggagePiece::PrintInfo() const
{
    std::cout << "  Место багажа [" << m_owner << "] вес: " << m_weightKg << " кг, сумма габаритов: " << m_sizeSumCm
              << " см, бирка: " << m_tag.GetNumber() << ", статус: " << StatusToString(m_status) << "\n";
}

} // namespace baggage

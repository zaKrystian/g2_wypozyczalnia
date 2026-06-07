#include "Transaction.h"

Transaction::Transaction(int tId, int uId, int vId, const std::string& brand, const std::string& start, int startMil)
    : transactionId(tId), userId(uId), vehicleId(vId), vehicleBrand(brand), startDate(start), 
      rentalDays(0), startMileage(startMil), endMileage(0), 
      totalCost(0.0), extraFee(0.0), status(TransactionStatus::ACTIVE) {}

Transaction::Transaction(int tId, int uId, int vId, const std::string& brand, const std::string& start, 
                         int days, int startMil, int endMil, double total, double extra, TransactionStatus stat)
    : transactionId(tId), userId(uId), vehicleId(vId), vehicleBrand(brand), startDate(start), 
      rentalDays(days), startMileage(startMil), endMileage(endMil), 
      totalCost(total), extraFee(extra), status(stat) {}

void Transaction::completeTransaction(int days, int endMil, double total, double extra) {
    rentalDays = days;
    endMileage = endMil;
    totalCost = total;
    extraFee = extra;
    status = TransactionStatus::COMPLETED;
}

int Transaction::getTransactionId() const { return transactionId; }
int Transaction::getUserId() const { return userId; }
int Transaction::getVehicleId() const { return vehicleId; }
std::string Transaction::getVehicleBrand() const { return vehicleBrand; }
std::string Transaction::getStartDate() const { return startDate; }
int Transaction::getRentalDays() const { return rentalDays; }
int Transaction::getStartMileage() const { return startMileage; }
int Transaction::getEndMileage() const { return endMileage; }
double Transaction::getTotalCost() const { return totalCost; }
double Transaction::getExtraFee() const { return extraFee; }
TransactionStatus Transaction::getStatus() const { return status; }

std::string Transaction::getStatusAsString() const {
    return (status == TransactionStatus::ACTIVE) ? "ACTIVE" : "COMPLETED";
}

void Transaction::displaySummary() const {
    std::string statusStr = (status == TransactionStatus::ACTIVE) ? "AKTYWNA" : "ZAKONCZONA";
    std::cout << "[ID Transakcji: " << transactionId 
              << "] | Pojazd: " << vehicleBrand << " (ID: " << vehicleId << ")"
              << " | ID Uzytkownika: " << userId 
              << " | Data poczatkowa: " << startDate 
              << " | Przebieg poczatkowy: " << startMileage << " km"
              << " | Status: " << statusStr;
              
    if (status == TransactionStatus::COMPLETED) {
        std::cout << " | Czas trwania: " << rentalDays << " dni | Koszt: " << totalCost << " PLN";
    }
    std::cout << std::endl;
}
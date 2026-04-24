#include "Transaction.h"

Transaction::Transaction(int tId, int uId, int vId, std::string sDate, int sMileage)
    : transactionId(tId), userId(uId), vehicleId(vId), startDate(sDate), 
      startMileage(sMileage), endMileage(0), totalCost(0), extraFees(0) {}

void Transaction::completeTransaction(std::string eDate, int eMileage, double cost, double fees) {
    endDate = eDate;
    endMileage = eMileage;
    totalCost = cost;
    extraFees = fees;
}
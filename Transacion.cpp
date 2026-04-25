#include "Transaction.h"

using namespace std;

Transaction::Transaction(int tId, int uId, int vId, string sDate, int sMileage)
    : transactionId(tId), userId(uId), vehicleId(vId), startDate(sDate), 
      startMileage(sMileage), endMileage(0), totalCost(0), extraFees(0) {}

void Transaction::completeTransaction(string eDate, int eMileage, double cost, double fees) {
    endDate = eDate;
    endMileage = eMileage;
    totalCost = cost;
    extraFees = fees;
}
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
public:
    int transactionId;
    int userId;
    int vehicleId;
    std::string startDate;
    std::string endDate;
    int startMileage;
    int endMileage;
    double totalCost;
    double extraFees;

    Transaction(int tId, int uId, int vId, std::string sDate, int sMileage);
    void completeTransaction(std::string eDate, int eMileage, double cost, double fees);
};

#endif
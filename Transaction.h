#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
using namespace std;

class Transaction {
public:
    int transactionId;
    int userId;
    int vehicleId;
    string startDate;
    string endDate;
    int startMileage;
    int endMileage;
    double totalCost;
    double extraFees;

    Transaction(int tId, int uId, int vId, string sDate, int sMileage);
    void completeTransaction(string eDate, int eMileage, double cost, double fees);
};

#endif
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <iostream>

enum class TransactionStatus {
    ACTIVE,
    COMPLETED
};

class Transaction {
private:
    int transactionId;
    int userId;
    int vehicleId;
    std::string vehicleBrand;
    std::string startDate;
    int rentalDays; 
    int startMileage;
    int endMileage;
    double totalCost;
    double extraFee;
    TransactionStatus status;

public:
    Transaction(int tId, int uId, int vId, const std::string& brand, const std::string& start, int startMil);

    Transaction(int tId, int uId, int vId, const std::string& brand, const std::string& start, 
                int days, int startMil, int endMil, double total, double extra, TransactionStatus stat);

    void completeTransaction(int days, int endMil, double total, double extra);

    int getTransactionId() const;
    int getUserId() const;
    int getVehicleId() const;
    std::string getVehicleBrand() const;
    std::string getStartDate() const;
    int getRentalDays() const;
    int getStartMileage() const;
    int getEndMileage() const;
    double getTotalCost() const;
    double getExtraFee() const;
    TransactionStatus getStatus() const;
    std::string getStatusAsString() const;
    
    void displaySummary() const;
};

#endif
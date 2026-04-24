#ifndef RENTALSYSTEM_H
#define RENTALSYSTEM_H

#include <vector>
#include <iostream>
#include "Vehicle.h"
#include "User.h"
#include "Transaction.h"

class RentalSystem {
private:
    std::vector<Vehicle> vehicles;
    std::vector<User> users;
    std::vector<Transaction> transactions;
    int nextTransactionId = 1;

public:
    void addVehicle(const Vehicle& v);
    void addUser(const User& u);
    
    // MVP Logic
    bool rentVehicle(int vehicleId, int userId, std::string date);
    bool returnVehicle(int vehicleId, int currentMileage, std::string date, int days);
    
    void displayFleetStatus() const;
    void displayHistory() const;
};

#endif
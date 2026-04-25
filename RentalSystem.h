#ifndef RENTALSYSTEM_H
#define RENTALSYSTEM_H

#include <string>
#include <vector>
#include <iostream>
#include "Vehicle.h"
#include "User.h"
#include "Transaction.h"

using namespace std;

class RentalSystem {
private:
    vector<Vehicle> vehicles;
    vector<User> users;
    vector<Transaction> transactions;
    int nextTransactionId = 1;

public:
    void addVehicle(const Vehicle& v);
    void addUser(const User& u);
    
    // MVP Logic
    bool rentVehicle(int vehicleId, int userId, string date);
    bool returnVehicle(int vehicleId, int currentMileage, string date, int days);
    
    void displayFleetStatus() const;
    void displayHistory() const;

    void loadVehiclesFromCSV(const string& filename);
};

#endif
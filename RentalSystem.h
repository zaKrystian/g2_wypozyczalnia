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
    void rentVehicle(int vehicleId, int userId, std::string date);
    void returnVehicle(int vehicleId, int currentMileage, string date, int days);
    
    void displayFleetStatus() const;
    void displayFleetStatus(bool displayActive) const;
    void displayUsers() const;
    void displayHistory() const;

    void loadVehiclesFromCSV(const string& filename);
    void loadUsersFromCSV(const string& filename);
};

#endif
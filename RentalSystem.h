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
    User* currentUser = nullptr;

public:
    void addVehicle(const Vehicle& v);
    void addUser(const User& u);
    
    // MVP Logic
    void rentVehicle(int vehicleId, int userId, string date);
    void returnVehicle(int vehicleId, int currentMileage, int days);

    
    void displayFleetStatus() const;
    void displayFleetStatus(bool displayActive) const;
    void displayUsers() const;
    void displayHistory() const;

    void loadVehiclesFromCSV(const string& filename);
    

    void addNewVehicle(const std::string& brand, const std::string& model, int year, int mileage, int serviceLimit, double rate);

    void saveVehiclesToCSV(const std::string& filename) const;
    void loadUsersFromCSV(const std::string& filename);
    void saveUsersToCSV(const std::string& filename);

    bool login(const std::string& username, const std::string& password);
    void logout();
    User* getCurrentUser() const;

    
    // Nowe metody do obsługi widoków transakcji
    void displayAllTransactions() const;
    void displayUserTransactions(int userId) const;
    void displayActiveUserTransactions(int userId) const;
    bool hasActiveTransactions(int userId) const;

    void loadTransactionsFromCSV(const std::string& filename);
    void saveTransactionsToCSV(const std::string& filename) const;
    
};

#endif
#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

enum class Status { AVAILABLE, RENTED, MAINTENANCE };

class Vehicle {
public:
    int id;
    std::string brand;
    std::string model;
    int productionYear;
    int mileage;
    int serviceMileageLimit; // Limit przebiegu do serwisu
    Status status;
    double dailyRate;
    int mileageLimitPerRental; // Limit km na jedno wypozyczenie

    Vehicle(int id, std::string brand, std::string model, int year, int mileage, int serviceLimit, double rate);
    
    bool needsService() const;
    void updateMileage(int newMileage);
};

#endif

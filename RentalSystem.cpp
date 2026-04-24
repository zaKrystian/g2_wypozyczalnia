#include "RentalSystem.h"

void RentalSystem::addVehicle(const Vehicle& v) {
    vehicles.push_back(v);
}

void RentalSystem::addUser(const User& u) {
    users.push_back(u);
}

bool RentalSystem::rentVehicle(int vehicleId, int userId, std::string date) {
    for (auto &v : vehicles) {
        if (v.id == vehicleId) {
            // Blokada: Serwis lub zajety
            if (v.status != Status::AVAILABLE) {
                std::cout << "Error: Vehicle is not available!\n";
                return false;
            }
            if (v.needsService()) {
                v.status = Status::MAINTENANCE;
                std::cout << "Error: Vehicle requires maintenance!\n";
                return false;
            }

            v.status = Status::RENTED;
            transactions.push_back(Transaction(nextTransactionId++, userId, vehicleId, date, v.mileage));
            std::cout << "Vehicle " << v.brand << " rented successfully.\n";
            return true;
        }
    }
    return false;
}

bool RentalSystem::returnVehicle(int vehicleId, int currentMileage, std::string date, int days) {
    for (auto &v : vehicles) {
        if (v.id == vehicleId && v.status == Status::RENTED) {
            int distanceTraveled = currentMileage - v.mileage;
            double extraFee = 0;

            // Logika kar za przebieg
            if (distanceTraveled > v.mileageLimitPerRental) {
                extraFee = (distanceTraveled - v.mileageLimitPerRental) * 2.0; // 2.0 za kazdy km ponad limit
            }

            double total = (days * v.dailyRate) + extraFee;
            v.updateMileage(currentMileage);
            v.status = v.needsService() ? Status::MAINTENANCE : Status::AVAILABLE;

            // Aktualizacja ostatniej transakcji dla tego auta
            for (auto &t : transactions) {
                if (t.vehicleId == vehicleId && t.endMileage == 0) {
                    t.completeTransaction(date, currentMileage, total, extraFee);
                    break;
                }
            }

            std::cout << "Return successful. Total cost: " << total << " (Extra fees: " << extraFee << ")\n";
            return true;
        }
    }
    return false;
}

void RentalSystem::displayFleetStatus() const {
    std::cout << "\n--- Current Fleet Status ---\n";
    for (const auto &v : vehicles) {
        std::string s = (v.status == Status::AVAILABLE) ? "Available" : 
                        (v.status == Status::RENTED) ? "Rented" : "Maintenance";
        std::cout << v.brand << " " << v.model << " | Mileage: " << v.mileage << " | Status: " << s << "\n";
    }
}
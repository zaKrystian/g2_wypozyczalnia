#include "RentalSystem.h"
#include <fstream>
#include <sstream>

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
                std::cout << "Błąd: Brak pojazdu! Status:\n";
                return false;
            }
            if (v.needsService()) {
                v.status = Status::MAINTENANCE;
                std::cout << "Błąd: Pojazd jest w serwisie!\n";
                return false;
            }

            v.status = Status::RENTED;
            transactions.push_back(Transaction(nextTransactionId++, userId, vehicleId, date, v.mileage));
            std::cout << "Pojazd " << v.brand << " wypozyczony pomyślnie.\n";
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

            std::cout << "Zwrot pomyślny. Total cost: " << total << " (Dodatkowe opłaty: " << extraFee << ")\n";
            return true;
        }
    }
    return false;
}

void RentalSystem::displayFleetStatus() const {
    std::cout << "\n--- Obecny status floty ---\n";
    for (const auto &v : vehicles) {
        std::string s = (v.status == Status::AVAILABLE) ? "Dostępny" : 
                        (v.status == Status::RENTED) ? "Wypożyczony" : "W naprawie";
        std::cout << v.brand << " " << v.model << " | Przebieg: " << v.mileage << " | Status: " << s << "\n";
    }
}



// Funkcja pomocnicza do parsowania linii CSV (opcjonalnie)
void RentalSystem::loadVehiclesFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Błąd: Nie można otworzyć pliku " << filename << std::endl;
        return;
    }

    // Pomijamy nagłówek, jeśli istnieje w pliku
    std::getline(file, line); 

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> row;

        while (std::getline(ss, item, ',')) {
            row.push_back(item);
        }

        // Zakładany format CSV: id,brand,model,year,mileage,serviceLimit,dailyRate
        if (row.size() >= 7) {
            Vehicle v(
                std::stoi(row[0]), 
                row[1], 
                row[2], 
                std::stoi(row[3]), 
                std::stoi(row[4]), 
                std::stoi(row[5]), 
                std::stod(row[6])
            );
            addVehicle(v);
        }
    }
    file.close();
    std::cout << "Pomyślnie zaimportowano pojazdy z " << filename << "\n";
}


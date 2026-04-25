 #include "RentalSystem.h"
#include <iostream>

void RentalSystem::addVehicle(const Vehicle& v) {
    vehicles.push_back(v);
}

void RentalSystem::addUser(const User& u) {
    users.push_back(u);
}

bool RentalSystem::rentVehicle(int vehicleId, int userId, std::string date) {
    // 🔹 Szukanie użytkownika
    const User* userPtr = nullptr;
    for (const auto &u : users) {
        if (u.getId() == userId) {
            userPtr = &u;
            break;
        }
    }

    if (!userPtr) {
        std::cout << "Błąd: Użytkownik nie istnieje!\n";
        return false;
    }

    // 🔹 Limit aktywnych wypożyczeń (max 2)
    int activeRentals = 0;
    for (const auto &t : transactions) {
        if (t.userId == userId && t.endMileage == 0) {
            activeRentals++;
        }
    }

    if (activeRentals >= 2) {
        std::cout << "Błąd: Użytkownik osiągnął limit wypożyczeń (2 pojazdy)!\n";
        return false;
    }

    // 🔹 Szukanie pojazdu
    for (auto &v : vehicles) {
        if (v.id == vehicleId) {

            if (v.status != Status::AVAILABLE) {
                std::cout << "Błąd: Pojazd nie jest dostępny!\n";
                return false;
            }

            if (v.needsService()) {
                v.status = Status::MAINTENANCE;
                std::cout << "Błąd: Pojazd wymaga serwisu!\n";
                return false;
            }

            v.status = Status::RENTED;
            transactions.push_back(
                Transaction(nextTransactionId++, userId, vehicleId, date, v.mileage)
            );

            std::cout << "Pojazd " << v.brand << " został wypożyczony pomyślnie.\n";
            return true;
        }
    }

    std::cout << "Błąd: Nie znaleziono pojazdu!\n";
    return false;
}

bool RentalSystem::returnVehicle(int vehicleId, int currentMileage, std::string date, int days) {
    for (auto &v : vehicles) {
        if (v.id == vehicleId && v.status == Status::RENTED) {

            int distanceTraveled = currentMileage - v.mileage;
            double extraFee = 0;

            // 🔹 kara za przekroczenie limitu
            if (distanceTraveled > v.mileageLimitPerRental) {
                extraFee = (distanceTraveled - v.mileageLimitPerRental) * 2.0;
            }

            double total = (days * v.dailyRate) + extraFee;

            // 🔹 znajdź użytkownika do zniżki
            int userId = -1;
            for (auto &t : transactions) {
                if (t.vehicleId == vehicleId && t.endMileage == 0) {
                    userId = t.userId;
                    break;
                }
            }

            // 🔹 sprawdź czy premium
            bool isPremium = false;
            for (const auto &u : users) {
                if (u.getId() == userId) {
                    isPremium = u.getIsPremium();
                    break;
                }
            }

            // 🔹 zniżka 10% dla premium
            if (isPremium) {
                total *= 0.9;
            }

            v.updateMileage(currentMileage);
            v.status = v.needsService() ? Status::MAINTENANCE : Status::AVAILABLE;

            // 🔹 aktualizacja transakcji
            for (auto &t : transactions) {
                if (t.vehicleId == vehicleId && t.endMileage == 0) {
                    t.completeTransaction(date, currentMileage, total, extraFee);
                    break;
                }
            }

            std::cout << "Zwrot zakończony. Całkowity koszt: " << total
                      << " (Dodatkowe opłaty: " << extraFee << ")";
            
            if (isPremium) {
                std::cout << " [Zastosowano zniżkę premium]";
            }

            std::cout << "\n";
            return true;
        } 
    }

    std::cout << "Błąd: Nie znaleziono pojazdu lub nie jest wypożyczony!\n";
    return false;
}

void RentalSystem::displayFleetStatus() const {
    std::cout << "\n--- Aktualny stan floty ---\n";
    for (const auto &v : vehicles) {
        std::string s = (v.status == Status::AVAILABLE) ? "Dostępny" : 
                        (v.status == Status::RENTED) ? "Wypożyczony" : "Serwis";

        std::cout << v.brand << " " << v.model
                  << " | Przebieg: " << v.mileage
                  << " | Status: " << s << "\n";
    }
}
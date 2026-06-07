#include "RentalSystem.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept> // Potrzebne do rzucania wyjątków


using namespace std;

void RentalSystem::addVehicle(const Vehicle& v) {
    vehicles.push_back(v);
}

void RentalSystem::addUser(const User& u) {
    users.push_back(u);
}

// Zmieniono typ zwracany na void - błędy są teraz wyjątkami
void RentalSystem::rentVehicle(int vehicleId, int userId, std::string date) {
    bool vehicleFound = false;

    // Sprawdzamy najpierw, czy użytkownik w ogóle istnieje w systemie
    bool userFound = false;
    for (const auto &u : users) {
        if (u.getId() == userId) {
            userFound = true;
            break;
        }
    }
    if (!userFound) {
        throw std::runtime_error("Uzytkownik o podanym ID nie istnieje w bazie.");
    }

    for (auto &v : vehicles) {
        if (v.id == vehicleId) {
            vehicleFound = true;

            // Blokada: Serwis lub zajety
            if (v.status != Status::AVAILABLE) {
                throw std::runtime_error("Pojazd nie jest dostepny (jest juz wypozyczony lub w naprawie).");
            }
            if (v.needsService()) {
                v.status = Status::MAINTENANCE;
                throw std::runtime_error("Pojazd wymaga natychmiastowego serwisu! Zmieniono status na MAINTENANCE.");
            }

            // Jeśli wszystko OK:
            v.status = Status::RENTED;
            transactions.push_back(Transaction(nextTransactionId++, userId, vehicleId, date, v.mileage));
            return; // Sukces, wychodzimy z metody
        }
    }

    if (!vehicleFound) {
        throw std::runtime_error("Pojazd o podanym ID nie istnieje w bazie.");
    }
}

void RentalSystem::returnVehicle(int vehicleId, int currentMileage, std::string date, int days) {
    for (auto &v : vehicles) {
        if (v.id == vehicleId) {
            if (v.status != Status::RENTED) {
                throw std::runtime_error("Ten pojazd nie jest obecnie oznaczony jako wypozyczony.");
            }
            if (currentMileage < v.mileage) {
                throw std::invalid_argument("Nowy przebieg nie moze byc mniejszy niz przebieg poczatkowy!");
            }

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

            std::cout << " #_SUKCES_# :  Zwrot pomyslny. Calkowity koszt: " << total << " (Dodatkowe oplaty: " << extraFee << ")\n";
        }
    }
    throw std::runtime_error("Nie znaleziono pojazdu o podanym ID.");
}

void RentalSystem::displayFleetStatus() const {
    std::cout << "\n>--- Obecny status floty ---<\n";
    for (const auto &v : vehicles) {
        std::string s = (v.status == Status::AVAILABLE) ? "Dostepny" : 
                        (v.status == Status::RENTED) ? "Wypozyczony" : "W naprawie";
        std::cout << v.brand << " " << v.model << " | Przebieg: " << v.mileage << " | Status: " << s << "\n";
    }
}

void RentalSystem::displayFleetStatus(bool displayAvailable) const {
    std::cout << "\n>--- Dostepne pojazdy ---<" << endl;
    for (const auto &v : vehicles) {
        if(v.status == Status::AVAILABLE){
            std::cout << "ID: " << v.id << " | " << v.brand << " " << v.model << " | Przebieg: " << v.mileage << " \n";
        }
    }
    cout << endl;
}

void RentalSystem::displayUsers() const {
    cout << "\n>--- Lista Uzytkownikow ---<" << endl;
    for (const auto& user : users) {
        cout << "ID: " << user.getId() << " | Imie: " << user.getName() << endl;
    }
    cout << endl;
}

void RentalSystem::loadVehiclesFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        // Zamiast std::cerr rzucamy wyjątek, który zamknie aplikację w main, jeśli krytyczne pliki znikną
        throw std::runtime_error("Nie mozna otworzyc pliku bazy pojazdow: " + filename);
    }

    std::getline(file, line); 

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> row;

        while (std::getline(ss, item, ',')) {
            row.push_back(item);
        }

        if (row.size() >= 7) {
            try {
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
            } catch (const std::exception& e) {
                std::cerr << "!_WARNING_! : Blad parsowania pojazdu w linii: " << line << " (" << e.what() << ")" << std::endl;
            }
        }
    }
    file.close();
    std::cout << "#_SUKCES_# : Pomyslnie zaimportowano pojazdy z " << filename << "\n";
}

void RentalSystem::loadUsersFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        throw std::runtime_error("Nie mozna otworzyc pliku bazy uzytkownikow: " + filename);
    }

    std::getline(file, line); 

    while (std::getline(file, line)) {
        if (line.empty()) continue; 
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> row;

        while (std::getline(ss, item, ',')) {
            row.push_back(item);
        }

        if (row.size() >= 3) {
            try {
                int id = std::stoi(row[0]);
                std::string name = row[1];
                bool isPremium = (row[2] == "1" || row[2] == "true");

                User u(id, name, isPremium);
                addUser(u); 
            } catch (const std::exception& e) {
                std::cerr << "!_ERROR_! : Blad parsowania uzytkownika w linii: " << line << " (" << e.what() << ")" << std::endl;
            }
        }
    }
    file.close();
    std::cout << "#_SUKCES_# : Pomyslnie zaimportowano użytkownikow z " << filename << "\n";
}

void RentalSystem::addNewVehicle(const std::string& brand, const std::string& model, int year, int mileage, int serviceLimit, double rate) {
    // Walidacja logiki domenowej
    if (brand.empty() || model.empty()) {
        throw std::invalid_argument("Marka oraz model pojazdu nie moga byc puste.");
    }
    if (year < 1900 || year > 2026) {
        throw std::invalid_argument("Podano nierealistyczny rok produkcji.");
    }
    if (mileage < 0) {
        throw std::invalid_argument("Przebieg nie moze byc wartoscia ujemna.");
    }
    if (serviceLimit <= mileage) {
        throw std::invalid_argument("Limit serwisowy musi przewyzszac aktualny przebieg.");
    }
    if (rate <= 0) {
        throw std::invalid_argument("Stawka dobowa musi byc wieksza od zera.");
    }

    // Automatyczne generowanie unikalnego ID
    int nextId = 1;
    for (const auto& v : vehicles) {
        if (v.id >= nextId) {
            nextId = v.id + 1;
        }
    }

    // Utworzenie obiektu i dodanie do wektora bazy
    Vehicle newVehicle(nextId, brand, model, year, mileage, serviceLimit, rate);
    addVehicle(newVehicle);
}

void RentalSystem::saveVehiclesToCSV(const std::string& filename) const {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Nie mozna otworzyc pliku do zapisu bazy pojazdow: " + filename);
    }

    // Zapis nagłówka (niezbędne, gdyż loadVehiclesFromCSV ignoruje pierwszą linię)
    file << "id,brand,model,year,mileage,serviceLimit,rate\n";

    for (const auto& v : vehicles) {
        file << v.id << "," 
             << v.brand << "," 
             << v.model << "," 
             << v.productionYear << "," 
             << v.mileage << "," 
             << v.serviceMileageLimit << "," 
             << v.dailyRate << "\n";
    }
    
    file.close();
}

void RentalSystem::saveUsersToCSV(const std::string& filename) const {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Nie mozna otworzyc pliku do zapisu bazy uzytkownikow: " + filename);
    }

    // Zapis nagłówka
    file << "id,name,isPremium\n";

    for (const auto& u : users) {
        file << u.getId() << "," 
             << u.getName() << "," 
             << (u.getIsPremium() ? "1" : "0") << "\n"; 
    }
    
    file.close();
}
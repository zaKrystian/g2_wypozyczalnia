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
void RentalSystem::rentVehicle(int vehicleId, int userId, string date) {
    bool vehicleFound = false;

    
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
                return;
            }
            if (v.needsService()) {
                v.status = Status::MAINTENANCE;
                throw std::runtime_error("Pojazd wymaga natychmiastowego serwisu! Zmieniono status na MAINTENANCE.");
                return;
            }

            // Jeśli wszystko OK:
            v.status = Status::RENTED;
            transactions.push_back(Transaction(nextTransactionId++, userId, vehicleId, v.brand, date, v.mileage));
            return; 
        }
    }

    if (!vehicleFound) {
        throw std::runtime_error("Pojazd o podanym ID nie istnieje w bazie.");
    }
}

void RentalSystem::returnVehicle(int vehicleId, int currentMileage, int days) {
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

            if (distanceTraveled > v.mileageLimitPerRental) {
                extraFee = (distanceTraveled - v.mileageLimitPerRental) * 2.0;
            }

            double total = (days * v.dailyRate) + extraFee;
            v.updateMileage(currentMileage);
            v.status = v.needsService() ? Status::MAINTENANCE : Status::AVAILABLE;

            // Aktualizacja aktywnej transakcji dla tego auta
            bool transactionFound = false;
            for (auto &t : transactions) {
                if (t.getVehicleId() == vehicleId && t.getStatus() == TransactionStatus::ACTIVE) {
                    t.completeTransaction(days, currentMileage, total, extraFee);
                    transactionFound = true;
                    break;
                }
            }

            if (!transactionFound) {
                throw std::runtime_error("Nie znaleziono aktywnej transakcji dla tego pojazdu.");
            }

            std::cout << " #_SUKCES_# :  Zwrot pomyslny. Calkowity koszt: " << total << " (Dodatkowe oplaty: " << extraFee << ")\n";
            return; 
        }
    }
    throw std::runtime_error("Nie znaleziono pojazdu o podanym ID.");
}


void RentalSystem::displayAllTransactions() const {
    std::cout << "\n>--- Rejestr wszystkich transakcji (Tryb Administratora) ---<\n";
    if (transactions.empty()) {
        std::cout << "Brak zapisanych transakcji w systemie.\n";
        return;
    }
    for (const auto& t : transactions) {
        t.displaySummary();
    }
}

void RentalSystem::displayUserTransactions(int userId) const {
    std::cout << "\n>--- Historia transakcji uzytkownika ---<\n";
    bool found = false;
    for (const auto& t : transactions) {
        if (t.getUserId() == userId) {
            t.displaySummary();
            found = true;
        }
    }
    if (!found) {
        std::cout << "Brak historii transakcji dla tego uzytkownika.\n";
    }
}

void RentalSystem::displayActiveUserTransactions(int userId) const {
    std::cout << "\n>--- Aktywne wypozyczenia ---<\n";
    for (const auto& t : transactions) {
        if (t.getUserId() == userId && t.getStatus() == TransactionStatus::ACTIVE) {
            t.displaySummary();
        }
    }
}

bool RentalSystem::hasActiveTransactions(int userId) const {
    for (const auto& t : transactions) {
        if (t.getUserId() == userId && t.getStatus() == TransactionStatus::ACTIVE) {
            return true;
        }
    }
    return false;
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

void RentalSystem::displayFleetStatus(bool adminPowers, bool displayUnAvailable) const {
    std::cout << "\n>--- Dostepne pojazdy ---<" << endl;
    for (const auto &v : vehicles) {
        if(v.status == Status::MAINTENANCE){
            std::cout << "ID: " << v.id << " | " << v.brand << " " << v.model << " | Przebieg: " << v.mileage << " \n";
        }
    }
    cout << endl;
}


void RentalSystem::displayUsers() const {
    cout << "\n>--- Lista Uzytkownikow ---<" << endl;
    for (const auto& user : users) {
        cout << "ID: " << user.getId() << " | Imie: " << user.getUsername() << endl;
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



void RentalSystem::loadUsersFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // Awaryjne tworzenie admina, jeśli baza danych użytkowników jest pusta
        users.push_back(User(1, "admin", "admin123", UserRole::ADMIN));
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string idStr, username, password, roleStr;

        std::getline(ss, idStr, ',');
        std::getline(ss, username, ',');
        std::getline(ss, password, ',');
        std::getline(ss, roleStr, ',');

        int id = std::stoi(idStr);
        UserRole role = (roleStr == "ADMIN") ? UserRole::ADMIN : UserRole::CLIENT;

        users.push_back(User(id, username, password, role));
    }
    file.close();
}

void RentalSystem::saveUsersToCSV(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    for (const auto& user : users) {
        file << user.getId() << ","
             << user.getUsername() << ","
             << user.getPassword() << ","
             << user.getRoleAsString() << "\n";
    }
    file.close();
}

bool RentalSystem::login(const std::string& username, const std::string& password) {
    for (auto& user : users) {
        if (user.getUsername() == username && user.checkPassword(password)) {
            currentUser = &user;
            return true;
        }
    }
    return false;
}

void RentalSystem::logout() {
    currentUser = nullptr;
}

User* RentalSystem::getCurrentUser() const {
    return currentUser;
}

void RentalSystem::loadTransactionsFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }

    std::string line;
    std::getline(file, line); // Pominięcie nagłówka

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> row;

        while (std::getline(ss, item, ',')) {
            row.push_back(item);
        }

        if (row.size() >= 11) {
            try {
                int tId = std::stoi(row[0]);
                int uId = std::stoi(row[1]);
                int vId = std::stoi(row[2]);
                std::string brand = row[3];
                std::string startD = row[4];
                int days = std::stoi(row[5]); // Parsowanie liczby dni zamiast daty tekstowej
                int startM = std::stoi(row[6]);
                int endM = std::stoi(row[7]);
                double cost = std::stod(row[8]);
                double fee = std::stod(row[9]);
                TransactionStatus stat = (row[10] == "ACTIVE") ? TransactionStatus::ACTIVE : TransactionStatus::COMPLETED;

                transactions.push_back(Transaction(tId, uId, vId, brand, startD, days, startM, endM, cost, fee, stat));
                
                if (tId >= nextTransactionId) {
                    nextTransactionId = tId + 1;
                }
            } catch (const std::exception& e) {
                std::cerr << "!_WARNING_! : Blad parsowania transakcji w linii: " << line << " (" << e.what() << ")" << std::endl;
            }
        }
    }
    file.close();
}

void RentalSystem::saveTransactionsToCSV(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Blad: Nie mozna otworzyc pliku do zapisu transakcji: " << filename << "\n";
        return;
    }

    file << "transactionId,userId,vehicleId,vehicleBrand,startDate,rentalDays,startMileage,endMileage,totalCost,extraFee,status\n";

    for (const auto& t : transactions) {
        file << t.getTransactionId() << ","
             << t.getUserId() << ","
             << t.getVehicleId() << ","
             << t.getVehicleBrand() << ","
             << t.getStartDate() << ","
             << t.getRentalDays() << ","
             << t.getStartMileage() << ","
             << t.getEndMileage() << ","
             << t.getTotalCost() << ","
             << t.getExtraFee() << ","
             << t.getStatusAsString() << "\n";
    }
    file.close();
}

void RentalSystem::repairVehicle(int vehicleId) {
    for (auto &v : vehicles) {
        if (v.id == vehicleId) {
            if (v.status != Status::MAINTENANCE) {
                throw std::runtime_error("Pojazd nie znajduje sie obecnie w stanie serwisu/naprawy.");
            }

            // Zmiana statusu na dostepny we flocie
            v.status = Status::AVAILABLE;

            // Aktualizacja limitu serwisu o 10000 km od obecnego przebiegu pojazdu
            v.serviceMileageLimit = v.mileage + 10000;

            std::cout << " #_SUKCES_# : Pojazd o ID " << vehicleId << " zostal pomyslnie naprawiony i przywrocony do floty.\n";
            std::cout << "              Aktualny przebieg: " << v.mileage << " km | Nowy limit serwisu: " << v.serviceMileageLimit << " km\n";
            return;
        }
    }
    throw std::runtime_error("Nie znaleziono pojazdu o podanym ID.");
}
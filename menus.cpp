#include <iostream>
#include <limits> 
#include <stdexcept> 
#include "RentalSystem.h"

void systemHalt() {
    cin.clear(); 
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\n-> Nacisnij ENTER aby kontynuowac... ";
    cin.get(); 
}

void showGuestMenu() {
    std::cout << "\n=====================================\n";
    std::cout << "     SYSTEM WYPOZYCZALNI POJAZDOW    \n";
    std::cout << "=====================================\n";
    std::cout << "1. Zaloguj sie\n";
    std::cout << "0. Wyjdz z programu\n";
    std::cout << "-------------------------------------\n";
    std::cout << "Wybor: ";
}

void showClientMenu(const std::string& name) {
    std::cout << "\n=====================================\n";
    std::cout << "  PANEL KLIENTA: " << name << "\n";
    std::cout << "=====================================\n";
    std::cout << "1. Przegladaj dostepne pojazdy\n";
    std::cout << "2. Wypozycz pojazd\n";
    std::cout << "3. Zwroc pojazd\n";
    std::cout << "4. Wyswietl moja historie wypozyczen\n";
    std::cout << "5. Wyloguj sie\n";
    std::cout << "0. Wyjdz z programu\n";
    std::cout << "-------------------------------------\n";
    std::cout << "Wybor: ";
}

void showAdminMenu(const std::string& name) {
    std::cout << "\n=====================================\n";
    std::cout << "  PANEL ADMINISTRATORA: " << name << "\n";
    std::cout << "=====================================\n";
    std::cout << "1. Wyswietl pelna flote pojazdow\n";
    std::cout << "2. Dodaj nowy pojazd do systemu\n";
    std::cout << "3. Usun pojazd z systemu\n";
    std::cout << "4. Przegladaj wszystkie transakcje\n";
    std::cout << "5. Wyloguj sie\n";
    std::cout << "0. Wyjdz z programu\n";
    std::cout << "-------------------------------------\n";
    std::cout << "Wybor: ";
}
#include <iostream>
#include "RentalSystem.h"
#include "menus.h"
#include <limits> 
#include <stdexcept> 


int main() {
    RentalSystem system;

    // Bezpieczne ladowanie danych struktury CSV
    try {
        system.loadUsersFromCSV("data/users.csv");
        system.loadVehiclesFromCSV("data/vehicles.csv");
        system.loadTransactionsFromCSV("data/transactions.csv"); 
        
    } catch (const std::exception& e) {
        std::cerr << "Blad krytyczny podczas inicjalizacji danych: " << e.what() << "\n";
    }

    bool running = true;
    while (running) {
        User* loggedUser = system.getCurrentUser();

        // 1. MENU DLA NIEZALOGOWANEGO UZYTKOWNIKA (GOSC)
        if (loggedUser == nullptr) {
            showGuestMenu();
            int choice;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Niepoprawny znak! Wprowadz cyfre.\n";
                continue;
            }

            switch (choice) {
                case 1: {
                    std::string username, password;
                    std::cout << "Podaj login: ";
                    std::cin >> username;
                    std::cout << "Podaj haslo: ";
                    std::cin >> password;

                    if (system.login(username, password)) {
                        std::cout << "\nZalogowano pomyslnie! Witaj " << system.getCurrentUser()->getUsername() << ".\n";
                    } else {
                        std::cout << "\nBlad: Niepoprawny login lub haslo.\n";
                    }
                    break;
                }
                case 0:
                    running = false;
                    break;
                default:
                    std::cout << "Niepoprawna opcja menu.\n";
            }
        } 
        // 2. MENU DLA KLIENTA
        else if (loggedUser->getRole() == UserRole::CLIENT) {
            showClientMenu(loggedUser->getUsername());
            int choice;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                continue;
            }

            switch (choice) {
                case 1:
                    system.displayFleetStatus(true);
                    systemHalt();
                    break;
                case 2:
                    try {
                        system.displayFleetStatus(true); 
                        int rentID;
        
                        cout << "> Podaj ID samochodu do wypozyczenia: ";
                        if (!(cin >> rentID)) throw invalid_argument("Niepoprawny format ID samochodu (musi byc liczba).");

                        // Pobieranie ID bezpośrednio z sesji użytkownika
                        int sessionUserId = loggedUser->getId();
                        
                        system.rentVehicle(rentID, sessionUserId, "2026-05-08");
                        cout << " #_SUKCES_# : Pojazd wypozyczony pomyslnie." << endl;
                    } 
                    catch (const invalid_argument& e) {
                        cout << " !_ERROR_ danych wejsciowych: " << e.what() << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    catch (const exception& e) {
                        cout << " !_ERROR_ SYSTEMU: " << e.what() << endl;
                    }
                    systemHalt();
                    break;

                case 3:
                    try {
                        cout << "\n>--- Procedura Zwrotu Pojazdu ---<" << endl;
                        
                        int sessionUserId = loggedUser->getId();
                        
                        // Sprawdzenie warunku koniecznego: czy użytkownik posiada sprzęt do zwrotu
                        if (!system.hasActiveTransactions(sessionUserId)) {
                            cout << "Brak aktywnych wypozyczen. Procedura zwrotu zostala przerwana.\n";
                            systemHalt();
                            break;
                        }

                        system.displayActiveUserTransactions(sessionUserId);

                        int returnCarID, currentMileage, rentalDays;
                        
                        cout << "> Podaj ID samochodu: ";
                        if (!(cin >> returnCarID)) throw invalid_argument("Niepoprawny format ID transakcji (musi byc liczba).");

                        cout << "> Podaj obecny stan licznika (przebieg): ";
                        if (!(cin >> currentMileage)) throw invalid_argument("Niepoprawny format przebiegu (musi byc liczba).");

                        cout << "> Podaj liczbe dni wypozyczenia: ";
                        if (!(cin >> rentalDays)) throw invalid_argument("Niepoprawna liczba dni (musi byc liczba).");
                        if (rentalDays <= 0) throw invalid_argument("Liczba dni musi byc wieksza od 0.");

                        system.returnVehicle(returnCarID, currentMileage, rentalDays);
                    } 
                    catch (const invalid_argument& e) {
                        cout << " !_ERROR_ danych wejsciowych: " << e.what() << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    catch (const exception& e) {
                        cout << " !_ERROR_ SYSTEMU ZWROTOW: " << e.what() << endl;
                    }

                    systemHalt();
                    break;
                case 4:
                    system.displayUserTransactions(loggedUser->getId());
                    systemHalt();
                    break;
                case 5:
                    system.logout();
                    std::cout << "Wylogowano pomyslnie.\n";
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    std::cout << "Niepoprawna opcja menu.\n";
            }
        } 
        // 3. MENU DLA ADMINISTRATORA
        else if (loggedUser->getRole() == UserRole::ADMIN) {
            showAdminMenu(loggedUser->getUsername());
            int choice;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                continue;
            }

            switch (choice) {
                case 1:
                    system.displayFleetStatus();
                    systemHalt();
                    break;
                case 2:
                try {
                    string brand, model;
                    int year, mileage, serviceLimit;
                    double rate;

                    cout << "\n>--- Procedura Dodawania Nowego Pojazdu ---<" << endl;

                    // Oczyszczenie bufora po wczytaniu wyboru z menu
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "> Podaj marke pojazdu: ";
                    getline(cin, brand);

                    cout << "> Podaj model pojazdu: ";
                    getline(cin, model);

                    cout << "> Podaj rok produkcji: ";
                    if (!(cin >> year)) throw invalid_argument("Wymagana wartosc liczbowa dla roku.");

                    cout << "> Podaj obecny przebieg (km): ";
                    if (!(cin >> mileage)) throw invalid_argument("Wymagana wartosc liczbowa dla przebiegu.");

                    cout << "> Podaj limit przebiegu do serwisu (km): ";
                    if (!(cin >> serviceLimit)) throw invalid_argument("Wymagana wartosc liczbowa dla limitu.");

                    cout << "> Podaj stawke dobowa (PLN): ";
                    if (!(cin >> rate)) throw invalid_argument("Wymagana wartosc liczbowa dla stawki.");

                    // Przekazanie danych do warstwy logiki
                    system.addNewVehicle(brand, model, year, mileage, serviceLimit, rate);
                    
                    cout << "\n #_SUKCES_# : Pojazd zostal pomyslnie zarejestrowany w systemie." << endl;

                }
                catch (const invalid_argument& e) {
                    cout << " !_ERROR_ Danych: " << e.what() << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                catch (const exception& e) {
                    cout << " !_ERROR_ Krytyczny: " << e.what() << endl;
                }

                systemHalt();
                break;
                case 3: 
                    try {
                        int repairID;
                        cout << "\n>--- Procedura Serwisowa / Naprawa Pojazdu ---<" << endl;
                        system.displayFleetStatus(true,true);
                        cout << "> Podaj ID samochodu, ktory zostal naprawiony: ";
                        if (!(cin >> repairID)) {
                            throw invalid_argument("Niepoprawny format ID samochodu (musi byc liczba).");
                        }

                        system.repairVehicle(repairID);
                    }
                    catch (const invalid_argument& e) {
                        cout << " !_ERROR_ danych wejsciowych: " << e.what() << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    catch (const exception& e) {
                        cout << " !_ERROR_ SYSTEMU SERWISOWEGO: " << e.what() << endl;
                    }

                    systemHalt();
                    break;
                case 4:
                    system.displayAllTransactions();
                    systemHalt();
                    break;
                case 5:
                    system.logout();
                    cout << "Wylogowano pomyslnie.\n";
                    break;
                case 0:
                    running = false;
                    break;
                default:
                    std::cout << "Niepoprawna opcja menu.\n";
            }
        }
    }

    // Bezpieczny zapis stanu bazy przed zamknieciem aplikacji
    try {
        system.saveUsersToCSV("data/users.csv");
        system.saveVehiclesToCSV("data/vehicles.csv");
        system.saveTransactionsToCSV("data/transactions.csv");
        
    } catch (const std::exception& e) {
        std::cerr << "Blad podczas zapisu danych do pliku CSV: " << e.what() << "\n";
    }

    std::cout << "\nProgram zakonczyl dzialanie. Do zobaczenia!\n";
    return 0;
}
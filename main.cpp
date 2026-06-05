#include "RentalSystem.h"
#include <iostream>
#include <limits> 
#include <stdexcept> 

using namespace std;

void systemHalt() {
    cin.clear(); 
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cout << "\n-> Nacisnij ENTER aby kontynuowac... ";
    cin.get(); 
}

int main() {

    RentalSystem system;
    
    
    try {
        system.loadVehiclesFromCSV("data/cars.csv");
        system.loadUsersFromCSV("data/users.csv");
    } 
    catch (const exception& e) {
        cerr << " CRITICAL ERROR: Nie udalo sie zainicjalizowac bazy danych!" << endl;
        cerr << " Szczegoly błędu: " << e.what() << endl;
        cerr << " Program zostanie zamkniety." << endl;
        return 1; 
    }

    int chosenState;
    bool run = true;

    while (run) {
        cout <<"\n";
        cout << "###################################" << endl;
        cout << "#     WYPORZYCZALNIA POJAZDOW     #"  << endl;
        cout << "###################################\n" << endl;
        cout << "> Wybierz akcje:" << endl;
        cout << " 1) Status Floty" << endl;
        cout << " 2) Wypozyczenie pojazdu" << endl;
        cout << " 3) Zwrot pojazdu" << endl;
        cout << " 4) Dodawanie pojazdu" << endl;
        cout << " 5) Dodawanie uzytkownika" << endl;
        cout << " 0) Konczenie dzialania programu" << endl;
        cout << "> WYBOR: ";

        if (!(cin >> chosenState)) {
            cout << " !_ERROR_!: To nie jest liczba. Sprobuj ponownie." << endl;
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue;
        }

        switch (chosenState) {
            case 1:
                system.displayFleetStatus();
                systemHalt();
                break;

            case 2:
                
                try {
                    system.displayFleetStatus(true); 
                    int rentID, userID;
    
                    cout << "> Podaj ID samochodu do wypozyczenia: ";
                    if (!(cin >> rentID)) throw invalid_argument("Niepoprawny format ID samochodu (musi byc liczba).");

                    system.displayUsers(); 
                    cout << "> Podaj ID użytkownika: ";
                    if (!(cin >> userID)) throw invalid_argument("Niepoprawny format ID uzytkownika (musi byc liczba).");

                    
                    system.rentVehicle(rentID, userID, "2026-05-08");
                    cout << " #_SUKCES_# : Pojazd wypozyczony pomyślnie." << endl;
                    
                } 
                catch (const invalid_argument& e) {
                    
                        cout << " !_ERROR_ danych wejściowych: " << e.what() << endl;
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
                    int returnID, currentMileage, rentalDays;
                    
                    cout << "\n>--- Procedura Zwrotu Pojazdu ---<" << endl;
                    cout << "> Podaj ID samochodu do zwrotu: ";
                    if (!(cin >> returnID)) throw invalid_argument("Niepoprawny format ID samochodu (musi byc liczba).");

                    cout << "> Podaj obecny stan licznika (przebieg): ";
                    if (!(cin >> currentMileage)) throw invalid_argument("Niepoprawny format przebiegu (musi byc liczba).");

                    cout << "> Podaj liczbe dni wypozyczenia: ";
                    if (!(cin >> rentalDays)) throw invalid_argument("Niepoprawna liczba dni (musi byc liczba).");
                    if (rentalDays <= 0) throw invalid_argument("Liczba dni musi byc wieksza od 0.");

                    system.returnVehicle(returnID, currentMileage, "2026-05-08", rentalDays);
                    
                } 
                catch (const invalid_argument& e) {
                   
                    cout << " !_ERROR_ danych wejściowych: " << e.what() << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                catch (const exception& e) {
                   
                    cout << " !_ERROR_ SYSTEMU ZWROTOW: " << e.what() << endl;
                }

                systemHalt();
                break;

            case 4:
                cout << "Logika dodawania pojazdu...." << endl;
                break;

            case 5:
                cout << "Logika dodawania uzytkownika..." << endl;
                break;

            case 0:
                cout << "Zamykanie systemu. Do widzenia!" << endl;
                run = false;
                break;

            default:
                cout << " !_ERROR_!: Podano niepoprawna liczbe (wybierz 0-5)." << endl;
                break;
        }
    }

    return 0;
}
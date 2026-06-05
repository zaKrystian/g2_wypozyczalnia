#include "RentalSystem.h"
#include <iostream>
#include <limits> 


using namespace std;

void systemHalt() {
    cin.clear(); 
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cout << "\n-> Nacisnij ENTER aby kontynuowac... ";
    cin.get(); 
}

int main() {

    RentalSystem system;
    system.loadVehiclesFromCSV("data/cars.csv");
    system.loadUsersFromCSV("data/users.csv");

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
                do {
                    system.displayFleetStatus(true); // Wyświetla dostępne auta
                    int rentID, userID;
    
                    cout << "> Podaj ID samochodu do wypozyczenia: ";
                    cin >> rentID;

                    system.displayUsers(); // Teraz zadziała bez argumentu
                    cout << "> Podaj ID użytkownika: ";
                    cin >> userID;

                    // Przekazujemy zmienne rentID oraz userID zamiast sztywnych "1"
                    
                    if (system.rentVehicle(rentID, userID, "2026-05-08")) {
                        cout << " #_SUKCES_# : Pojazd wypozyczony." << endl;
                        break;
                    } else {
                        cout << " !_ERROR_!: Nie udalo sie wypozyczyc pojazdu. Sprobuj ponownie." << endl;
                    }
                } while (true);
                systemHalt();
                
                break;

            case 3:
                cout << "Logika zwrotu pojazdu..." << endl;
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
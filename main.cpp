#include "RentalSystem.h"
#include <iostream>
#include <limits> 


using namespace std;

int main() {

    setlocale(LC_CTYPE, "Polish");
    RentalSystem system;
    system.loadVehiclesFromCSV("cars.csv");

    int chosenState;
    bool run = true;

    while (run) {
        cout << "\n###################################" << endl;
        cout << "*- Wypożyczalnia pojazdów -*" << endl;
        cout << " > Wybierz akcję:" << endl;
        cout << " 1) Status Floty" << endl;
        cout << " 2) Wypożyczenie pojazdu" << endl;
        cout << " 3) Zwrot pojazdu" << endl;
        cout << " 4) Dodawanie pojazdu" << endl;
        cout << " 5) Dodawanie użytkownika" << endl;
        cout << " 0) Kończenie działania programu" << endl;
        cout << "> Twój wybór: ";

    
        if (!(cin >> chosenState)) {
            cout << " !-BŁĄD-!: To nie jest liczba. Spróbuj ponownie." << endl;
            
            
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue;
        }

        switch (chosenState) {
            case 1:
                system.displayFleetStatus();
                break;

            case 2:
                system.rentVehicle(1, 1, "2026-05-08"); 
                break;

            case 3:
                cout << "Logika zwrotu pojazdu..." << endl;
                break;

            case 4:
                cout << "Logika dodawania pojazdu..." << endl;
                break;

            case 5:
                cout << "Logika dodawania użytkownika..." << endl;
                break;

            case 0:
                cout << "Zamykanie systemu. Do widzenia!" << endl;
                run = false;
                break;

            default:
                cout << " !-BŁĄD-!: Podano nieprawidłową liczbę (wybierz 0-5)." << endl;
                break;
        }
    }

    return 0;
}
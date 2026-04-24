#include "RentalSystem.h"

int main() {
    RentalSystem system;

    // Dodajemy auto z limitem serwisu na 10500 km i przebiegiem 10000 km
    Vehicle car1(1, "Tesla", "Model 3", 2023, 10000, 10500, 200);
    User user1(1, "John Doe", false);

    system.addVehicle(car1);
    system.addUser(user1);

    // 1. Pierwsze wypozyczenie
    system.rentVehicle(1, 1, "2026-04-24");
    
    // Zwrot z przekroczeniem limitu km (limit 500, przejechano 600)
    system.returnVehicle(1, 10600, "2026-04-25", 1);

    // 2. Proba ponownego wypozyczenia (powinno zablokowac przez serwis - przebieg 10600 > 10500)
    system.displayFleetStatus();
    system.rentVehicle(1, 1, "2026-04-26");

    return 0;
}
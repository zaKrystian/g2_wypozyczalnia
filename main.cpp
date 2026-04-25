#include "RentalSystem.h"
#include <iostream>
#include <fstream>



using namespace std;
int main() {
    RentalSystem system;

    
    system.loadVehiclesFromCSV("auta.csv");
    system.displayFleetStatus();
    system.rentVehicle(1, 1, "2026-04-26");

    return 0;
}

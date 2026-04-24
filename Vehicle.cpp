#include "Vehicle.h"

Vehicle::Vehicle(int id, std::string brand, std::string model, int year, int mileage, int serviceLimit, double rate)
    : id(id), brand(brand), model(model), productionYear(year), mileage(mileage), 
      serviceMileageLimit(serviceLimit), status(Status::AVAILABLE), dailyRate(rate), mileageLimitPerRental(500) {}

bool Vehicle::needsService() const {
    return mileage >= serviceMileageLimit;
}

void Vehicle::updateMileage(int newMileage) {
    if (newMileage > mileage) {
        mileage = newMileage;
    }
}
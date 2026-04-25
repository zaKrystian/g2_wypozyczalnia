#include "User.h"

User::User(int id, std::string name, bool isPremium)
    : id(id), name(name), isPremium(isPremium) {}
    int User::getId() const {
    return id;
}

std::string User::getName() const {
    return name;
}

bool User::getIsPremium() const {
    return isPremium;
}

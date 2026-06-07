#include "User.h"

User::User(int id, const std::string& username, const std::string& password, UserRole role)
    : id(id), username(username), password(password), role(role) {}

int User::getId() const { 
    return id; 
}

std::string User::getUsername() const { 
    return username; 
}

std::string User::getPassword() const { 
    return password; 
}

UserRole User::getRole() const { 
    return role; 
}

std::string User::getRoleAsString() const {
    return (role == UserRole::ADMIN) ? "ADMIN" : "CLIENT";
}

bool User::checkPassword(const std::string& inputPassword) const {
    return password == inputPassword;
}
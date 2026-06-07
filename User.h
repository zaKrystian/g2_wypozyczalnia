#ifndef USER_H
#define USER_H

#include <string>

// Definicja kategorii użytkowników
enum class UserRole {
    CLIENT,
    ADMIN
};

class User {
private:
    int id;
    std::string username;
    std::string password;
    UserRole role;

public:
    User(int id, const std::string& username, const std::string& password, UserRole role);

    // Gettery
    int getId() const;
    std::string getUsername() const;
    std::string getPassword() const;
    UserRole getRole() const;
    std::string getRoleAsString() const;

    // Metoda weryfikacji hasła
    bool checkPassword(const std::string& inputPassword) const;
};

#endif // USER_H
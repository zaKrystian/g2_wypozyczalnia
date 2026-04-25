#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    int id;
    std::string name;
    bool isPremium;

    User(int id, std::string name, bool isPremium);
    
    int getId() const;
    std::string getName() const;
    bool getIsPremium() const;
};

#endif
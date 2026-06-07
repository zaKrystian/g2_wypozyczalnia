#ifndef MENUS_H
#define MENUS_H

#include <string>

void systemHalt();
void showGuestMenu();
void showClientMenu(const std::string& name);
void showAdminMenu(const std::string& name);

#endif // MENUS_H
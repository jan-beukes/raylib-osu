#ifndef DATABASE_H
#define DATABASE_H

#include <string>

void initializeDatabase();
bool addUser(const std::string &username, const std::string &password);
bool checkUserCredentials(const std::string &username, const std::string &password);
void saveUserScore(const std::string &username, int score);

#endif // DATABASE_H

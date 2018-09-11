#include "database.hpp"

using std::string;

DataBase::DataBase(string username) : db_instance_(username + ".db") {}

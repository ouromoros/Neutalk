#ifndef SERVER_DB_
#define SERVER_DB_

#include <sqlite_modern_cpp.h>
#include <string>
#include <vector>

struct UserInfo {
  std::string username;
  std::string password;
};
class DataBase {
 public:
  static DataBase &GetInstance() {
    static DataBase db;
    return db;
  }

  bool SaveUserInfo(const UserInfo &);
  UserInfo GetUserInfo(const std::string &);
  bool UserExists(const std::string &);

  DataBase(DataBase const &) = delete;
  void operator=(DataBase const &) = delete;

 private:
  DataBase();
  sqlite::database db_instance_;
};

#endif

#include "database.hpp"

using std::string;

DataBase::DataBase() : db_instance_("server_data.db") {
  db_instance_ << "create table if not exists UserInfo ("
                  "   username text primary key,"
                  "   password text not null,"
                  ");";
}

bool DataBase::UserExists(const string& name) {
  int count;

  db_instance_ << "select count(*) from UserInfo where username = ?" << name >>
      count;

  return count == 1;
}

UserInfo DataBase::GetUserInfo(const string& name) {
  UserInfo info;
  info.username = name;
  db_instance_ << "select password from UserInfo where username = ?" << name >>
      info.password;
  return info;
}

bool DataBase::SaveUserInfo(const UserInfo& info) {
  db_instance_ << "insert into UserInfo (username,password) values (?,?)"
               << info.username << info.password;
  return true;
}

#ifndef CLIENT_DATABASE_
#define CLIENT_DATABASE_
#include <sqlite_modern_cpp.h>
#include <string>
#include <vector>

struct ChatRecord {
  std::string to;
  std::string from;
  std::string content;
};

class DataBase {
 public:
  DataBase(std::string);

  int SaveChatRecord(ChatRecord);
  std::vector<ChatRecord> GetChatRecords(std::string, std::string);

  DataBase(DataBase const&) = delete;
  void operator=(DataBase const&) = delete;

 private:
  sqlite::database db_instance_;
};
#endif

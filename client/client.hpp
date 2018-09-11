#ifndef CLIENT_CLIENT_
#define CLIENT_CLIENT_

#include <string>

#include "data_structure/cond_message_queue.hpp"
#include "data_structure/message_queue.hpp"

class Client {
 public:
  static Client& GetInstance() {
    static Client client;
    return client;
  }

  bool TryStart(const std::string& username, const std::string& password);

  bool RequestLogin(const std::string& username, const std::string& password);
  bool RequestRegister(const std::string& username,
                       const std::string& password);
  bool RequestSendMessage(const std::string& to_user,
                          const std::string& content);

  void ReceiveMessageThread();
  bool TryTakeMessageTo(const std::string& from_user, void (*handle)(Message));

  Client(Client const&) = delete;
  void operator=(Client const&) = delete;
  // void UpdateContactThread();

 private:
  bool logged_in_;
  std::string me;
  // DataBase *db_instance_;
  Client();
  CondMessageQueue message_queue_;
};

#endif

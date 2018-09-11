#ifndef SERVER_SERVER_
#define SERVER_SERVER_

#include <string>
#include <unordered_map>

#include <json.hpp>

#include "client_map.hpp"

class Server {
 public:
  Server();

  Server(Server const &) = delete;
  void operator=(Server const &) = delete;
  void Run();

 private:
  ClientMap client_map_;

  void ListenOnPassiveConnectionThread();
  void TrySetupActiveConnection(int);

  void ListenOnActiveConnectionThread();
  void HandleClientRequestThread(int);

  bool SendMessage(std::string, std::string);

  std::string RespondMessage(nlohmann::json);
  std::string RespondRegister(nlohmann::json);

  std::string TryRespondLogin(nlohmann::json);

  std::string RespondError();
};

#endif

#ifndef CLIENT_MAP_
#define CLIENT_MAP_

#include <mutex>
#include <string>
#include <unordered_map>

struct Client {
  std::string username;
  std::mutex mut;
  int passive_sockfd, active_sockfd;
  int cookie;
};

class ClientMap {
 public:
  ClientMap();

  void Put(std::string, Client *);
  Client *Get(std::string);
  void Remove(std::string);
  bool Exists(std::string);

 private:
  std::mutex mut_;
  std::unordered_map<std::string, Client *> client_map_;
};

#endif

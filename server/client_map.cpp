#include "client_map.hpp"

using std::lock_guard;
using std::string;

ClientMap::ClientMap() {}

void ClientMap::Put(string username, Client* cl) {
  lock_guard<std::mutex> lg(mut_);
  client_map_[username] = cl;
}

bool ClientMap::Exists(string username) {
  lock_guard<std::mutex> lg(mut_);
  return client_map_.find(username) == client_map_.end();
}

Client* ClientMap::Get(string username) {
  lock_guard<std::mutex> lg(mut_);
  return client_map_[username];
}

void ClientMap::Remove(string username) {
  lock_guard<std::mutex> lg(mut_);
  client_map_.erase(username);
}

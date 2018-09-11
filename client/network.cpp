#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <mutex>
#include <string>

#include "../shared/network.hpp"
#include "network.hpp"

using std::string;

Network::Network() {
  passive_sockfd_ = GetConnectSockfd(NULL, kServerPassivePortString);
  active_sockfd_ = GetConnectSockfd(NULL, kServerActivePortString);

  if (passive_sockfd_ == -1 || active_sockfd_ == -1) {
    exit(1);
  }
}

bool Network::Request(const string &req, string &resp) {
  static std::mutex mut;
  std::lock_guard<std::mutex> lg(mut);
  SendPacket(passive_sockfd_, req);

  if (!RecvPacket(passive_sockfd_, resp)) {
    // error
    return false;
  }
  return true;
}

bool Network::Request_(const string &req, string &resp) {
  SendPacket(active_sockfd_, req);

  if (!RecvPacket(passive_sockfd_, resp)) {
    // error
    return false;
  }
  return true;
}

bool Network::RecvMessage(string &mes) {
  static std::mutex mut;
  std::lock_guard<std::mutex> lg(mut);

  if (!RecvPacket(active_sockfd_, mes)) {
    // error
    return false;
  }
  return true;
}

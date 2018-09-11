#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

#include "network.hpp"

using std::string;

bool SendAll(const char *buf, int sockfd, int size) {
  int r;

  while (size) {
    r = send(sockfd, buf, size, 0);
    if (r <= 0) break;
    size -= r;
    buf += r;
  }

  return size == 0;
}

bool SendAll(const string &s, int sockfd) {
  return SendAll(s.c_str(), sockfd, s.size());
}

bool RecvAll(char *buf, int sockfd, int size) {
  int r;

  while (size) {
    r = recv(sockfd, buf, size, 0);
    if (r <= 0) break;
    buf += r;
    size -= r;
  }

  return size == 0;
}

bool SendPacket(int sockfd, const string &s) {
  int len, r;

  len = s.size();

  if (len < 0 || len > kMaxPacketSize) {
    return false;
  }

  if (!SendAll((char *)&len, sockfd, sizeof(int))) {
    return false;
  }

  if (!SendAll(s.c_str(), sockfd, len)) {
    return false;
  }

  return true;
}

bool RecvPacket(int sockfd, string &s) {
  int len, r;
  char buf[1000];

  if (!RecvAll((char *)&len, sockfd, sizeof(int))) {
    return false;
  }

  if (len < 0 || len > kMaxPacketSize) {
    return false;
  }

  if (!RecvAll(buf, sockfd, len)) {
    return false;
  }

  s = buf;
  return true;
}

int GetConnectSockfd(const char *addr, const char *port) {
  int status;
  addrinfo hints;
  addrinfo *servinfo;
  int s;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(addr, port, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    return -1;
  }

  s = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
  connect(s, servinfo->ai_addr, servinfo->ai_addrlen);

  return s;
}

int GetListenSockfd(const char *addr, const char *port) {
  int status, sockfd, r;
  addrinfo hints;
  addrinfo *servinfo;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(addr, port, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    return -1;
  }

  sockfd =
      socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

  int enable = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
  bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
  listen(sockfd, 128);

  return sockfd;
}

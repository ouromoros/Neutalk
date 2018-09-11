#ifndef SHARED_NETWORK_
#define SHARED_NETWORK_
#include <string>

const int kServerPassivePort = 35556;
const char* const kServerPassivePortString = "35556";
const int kServerActivePort = 35555;
const char* const kServerActivePortString = "35555";

const int kMaxPacketSize = 1000;

bool SendPacket(int, const std::string&);
bool RecvPacket(int, std::string&);
int GetConnectSockfd(const char*, const char*);
int GetListenSockfd(const char*, const char*);

#endif

#ifndef CLIENT_DATA_STRUCTURE_MESSAGE_
#define CLIENT_DATA_STRUCTURE_MESSAGE_
#include <string>

struct Message {
  std::string to;
  std::string from;
  std::string content;
};
#endif

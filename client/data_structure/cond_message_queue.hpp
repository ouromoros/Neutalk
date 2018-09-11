#ifndef CLIENT_DATA_STRUCTURE_COND_MESSAGE_QUEUE_
#define CLIENT_DATA_STRUCTURE_COND_MESSAGE_QUEUE_
#include <condition_variable>
#include <list>
#include <mutex>
#include <string>

#include "message.hpp"

class CondMessageQueue {
 public:
  void Push(Message);
  Message Pop(bool (*)(Message));
  bool TryPopTo(const std::string &from_user, void (*)(Message));

 private:
  std::mutex mut;
  std::condition_variable cond;
  std::list<Message> list;
};
#endif

#ifndef CLIENT_DATA_STRUCTURE_MESSAGE_QUEUE_
#define CLIENT_DATA_STRUCTURE_MESSAGE_QUEUE_
#include <condition_variable>
#include <list>
#include <mutex>
#include <string>

#include "message.hpp"

class MessageQueue {
 public:
  void Push(Message);
  Message Pop();

 private:
  std::mutex mut;
  std::condition_variable cond;
  std::list<Message> list;
};
#endif

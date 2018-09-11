#include "message_queue.hpp"

void MessageQueue::Push(Message m) {
  mut.lock();
  list.push_back(m);
  mut.unlock();
  cond.notify_one();
}

Message MessageQueue::Pop() {
  Message m;
  std::unique_lock<std::mutex> ul(mut);

  if (list.size()) {
    m = list.back();
    list.pop_back();
    ul.unlock();
    return m;
  } else {
    for (;;) {
      cond.wait(ul, []() -> bool { return true; });
      if (list.size()) {
        m = list.back();
        list.pop_back();
        ul.unlock();
        return m;
      }
    }
  }
}

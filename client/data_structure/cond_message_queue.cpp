#include "cond_message_queue.hpp"

void CondMessageQueue::Push(Message m) {
  mut.lock();
  list.push_back(m);
  mut.unlock();
  cond.notify_all();
}

Message CondMessageQueue::Pop(bool (*check)(Message)) {
  Message m;
  std::unique_lock<std::mutex> ul(mut);

  for (auto i = list.begin(); i != list.end(); i++) {
    if (check(*i)) {
      m = *i;
      list.erase(i);
      ul.unlock();
      return m;
    }
  }

  cond.wait(ul, [this, check]() -> bool { return check(list.back()); });

  m = list.back();
  list.pop_back();
  ul.unlock();

  return m;
}

bool CondMessageQueue::TryPopTo(const std::string &from_user,
                                void (*handle)(Message)) {
  Message m;
  mut.lock();

  for (auto i = list.begin(); i != list.end(); i++) {
    if (i->from == from_user) {
      m = *i;
      list.erase(i);
      mut.unlock();
      handle(m);
      return true;
    }
  }
  return false;
}

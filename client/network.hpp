#ifndef CLIENT_NETWORK_
#define CLIENT_NETWORK_
#include <string>

class Network {
 public:
  static Network& GetInstance() {
    static Network nw;
    return nw;
  }

  bool Request(const std::string& req, std::string& res);
  bool Request_(const std::string& req, std::string& res);
  bool RecvMessage(std::string& mes);

  Network(Network const&) = delete;
  void operator=(Network const&) = delete;

 private:
  int passive_sockfd_;
  int active_sockfd_;
  Network();
};
#endif

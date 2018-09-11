#include "client.hpp"

#include <assert.h>
#include <thread>

#include <json.hpp>
#include "data_structure/message.hpp"
#include "network.hpp"

using nlohmann::json;
using std::string;

Client::Client() : logged_in_(false) {}

bool Client::TryStart(const std::string& username,
                      const std::string& password) {
  bool success;
  success = RequestLogin(username, password);
  if (!success) {
    return false;
  }

  string resp;
  json req_j, resp_j;
  bool success_active;
  Network& nw = Network::GetInstance();

  req_j["username"] = username;
  req_j["password"] = password;
  nw.Request_(req_j.dump(), resp);
  resp_j = json::parse(resp);

  try {
    success_active = resp_j.at("success");
  } catch (...) {
    return false;
  }
  if (!success_active) {
    return false;
  }

  // init here
  me = username;
  logged_in_ = true;
  std::thread recv_thread(&Client::ReceiveMessageThread, this);
  recv_thread.detach();

  return true;
}

bool Client::RequestLogin(const std::string& username,
                          const std::string& password) {
  bool success;
  json req_j, content_j, resp_j;
  string resp;
  Network& nw = Network::GetInstance();

  req_j["type"] = "op/login";
  content_j["username"] = username;
  content_j["password"] = password;
  req_j["content"] = content_j;

  nw.Request(req_j.dump(), resp);

  resp_j = json::parse(resp);
  try {
    success = resp_j.at("success");
  } catch (...) {
    return false;
  }
  return false;
}

bool Client::RequestRegister(const std::string& username,
                             const std::string& password) {
  bool success;
  json req_j, content_j, resp_j;
  string resp;
  Network& nw = Network::GetInstance();

  req_j["type"] = "op/register";
  content_j["username"] = username;
  content_j["password"] = password;
  req_j["content"] = content_j;

  nw.Request(req_j.dump(), resp);

  resp_j = json::parse(resp);
  try {
    success = resp_j.at("success");
  } catch (...) {
    return false;
  }
  return false;
}

bool Client::RequestSendMessage(const std::string& to_user,
                                const std::string& content) {
  bool success;
  json req_j, content_j, resp_j;
  string resp;
  Network& nw = Network::GetInstance();

  assert(logged_in_ == true);

  req_j["type"] = "data/message";
  content_j["to"] = to_user;
  content_j["from"] = me;
  content_j["content"] = content;
  req_j["content"] = content_j;

  nw.Request(req_j.dump(), resp);

  resp_j = json::parse(resp);
  try {
    success = resp_j.at("success");
  } catch (...) {
    return false;
  }
  return false;
}

void Client::ReceiveMessageThread() {
  Network& nw = Network::GetInstance();
  string mes;
  json message_j, content_j;
  Message message;

  assert(logged_in_ == true);

  for (;;) {
    nw.RecvMessage(mes);
    message_j = json::parse(mes);

    try {
      assert(message_j["type"] == "data/message");

      content_j = message_j.at("content");
      message.to = content_j.at("to");
      message.from = content_j.at("from");
      message.content = content_j.at("content");
    } catch (...) {
      // log
      continue;
    }
    message_queue_.Push(message);
  }
}

bool Client::TryTakeMessageTo(const std::string& from_user,
                              void (*handle)(Message)) {
  return message_queue_.TryPopTo(from_user, handle);
}

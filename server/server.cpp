#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <mutex>
#include <thread>

#include <json.hpp>

#include "../shared/network.hpp"
#include "database.hpp"
#include "server.hpp"

using nlohmann::json;
using std::string;
using std::thread;

Server::Server() {}

void Server::Run() {
  thread passive_connection_thread(&Server::ListenOnPassiveConnectionThread,
                                   this);
  thread active_connection_thread(&Server::ListenOnActiveConnectionThread,
                                  this);

  active_connection_thread.join();
  passive_connection_thread.join();
}

void Server::ListenOnActiveConnectionThread() {
  int sockfd = GetListenSockfd(NULL, kServerActivePortString);
  for (;;) {
    int new_connection_fd = accept(sockfd, NULL, NULL);
    thread setup_active_connection_thread(&Server::TrySetupActiveConnection,
                                          this, new_connection_fd);
    setup_active_connection_thread.detach();
  }
}

void Server::TrySetupActiveConnection(int sockfd) {
  string validation, username, password;
  json validation_j;

  DataBase &db_instance = DataBase::GetInstance();

  RecvPacket(sockfd, validation);
  validation_j = json::parse(validation);

  try {
    username = validation_j.at("username");
    password = validation_j.at("password");
  } catch (...) {
    close(sockfd);
    return;
  }

  if (!client_map_.Exists(username)) {
    close(sockfd);
    return;
  }

  if (db_instance.GetUserInfo(username).password != password) {
    close(sockfd);
    return;
  }

  if (!client_map_.Get(username)->active_sockfd != -1) {
    close(sockfd);
    return;
  }

  client_map_.Get(username)->active_sockfd = sockfd;
}

void Server::ListenOnPassiveConnectionThread() {
  int sockfd = GetListenSockfd(NULL, kServerPassivePortString);
  for (;;) {
    int new_request_fd = accept(sockfd, NULL, NULL);
    thread handle_request_thread(&Server::HandleClientRequestThread, this,
                                 new_request_fd);
    handle_request_thread.detach();
  }
}

void Server::HandleClientRequestThread(int sockfd) {
  Client *cl;
  for (;;) {
    string req, resp;
    json req_j, resp_j;
    RecvPacket(sockfd, req);
    resp = TryRespondLogin(req);
    SendPacket(sockfd, resp);

    resp_j = json::parse(resp);
    if (resp_j["success"] == true) {
      req_j = json::parse(req);

      cl = new Client;
      cl->username = req_j["username"];
      cl->active_sockfd = -1;
      cl->passive_sockfd = sockfd;
      // cl->cookie = resp_j["content"]["cookie"];

      client_map_.Put(cl->username, cl);

      break;
    }
  }

  for (;;) {
    string req, type, resp;
    json req_j;

    RecvPacket(sockfd, req);
    req_j = json::parse(req);
    try {
      type = req_j.at("type");
    } catch (...) {
      resp = RespondError();
      SendPacket(sockfd, resp);
      continue;
    }

    if (type == "data/message") {
      resp = RespondMessage(req_j);
    } else if (type == "op/register") {
      resp = RespondRegister(req_j);
    }

    SendPacket(sockfd, resp);
  }
}

string Server::TryRespondLogin(json req) {
  string username, password;
  DataBase &db_instance = DataBase::GetInstance();
  json resp_j, resp_content_j;

  try {
    if (req.at("type") != "op/login") {
      return RespondError();
    }
  } catch (...) {
    return RespondError();
  }

  try {
    json content_j = req.at("content");
    username = content_j.at("username");
    password = content_j.at("password");
  } catch (...) {
    return RespondError();
  }

  if (!db_instance.UserExists(username)) {
    return RespondError();
  }

  if (db_instance.GetUserInfo(username).password != password) {
    return RespondError();
  }

  if (client_map_.Exists(username)) {
    return RespondError();
  }

  resp_j["success"] = true;
  // content_j["cookie"] = random serial;
  resp_j["content"] = resp_content_j;

  return resp_j.dump();
}

string Server::RespondRegister(json req) {
  string username, password;
  DataBase &db_instance = DataBase::GetInstance();
  json resp_j, resp_content_j;

  try {
    json content_j = req.at("content");
    username = content_j.at("username");
    password = content_j.at("password");
  } catch (...) {
    return RespondError();
  }

  if (db_instance.UserExists(username)) {
    return RespondError();
  }

  if (!db_instance.SaveUserInfo({username, password})) {
    return RespondError();
  }

  resp_j["success"] = true;
  resp_j["content"] = resp_content_j;

  return resp_j.dump();
}

string Server::RespondMessage(json req) {
  string to, from, content;
  json resp_j, resp_content_j;

  try {
    json content_j = req.at("content");
    to = content_j.at("to");
    from = content_j.at("from");
    content = content_j.at("content");
  } catch (...) {
    return RespondError();
  }

  if (!client_map_.Exists(to) || !client_map_.Exists(from)) {
    return RespondError();
  }

  if (!SendMessage(to, req)) {
    return RespondError();
  }

  resp_j["success"] = true;
  resp_j["content"] = resp_content_j;

  return resp_j.dump();
}

string Server::RespondError() {
  const static string error_resp = "{\"success\": false}";
  return error_resp;
}

bool Server::SendMessage(string to, string message) {
  Client *client = client_map_.Get(to);
  {
    std::lock_guard<std::mutex> lg(client->mut);
    SendPacket(client->active_sockfd, message);
  }
  return true;
}

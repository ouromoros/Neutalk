#ifndef CLIENT_USER_INTERFACE_
#define CLIENT_USER_INTERFACE_

#include <string>
#include <unordered_map>

#include "data_structure/message.hpp"
#include "ui/chat_window.hpp"
#include "ui/contact_window.hpp"
#include "ui/login_window.hpp"
#include "ui/register_window.hpp"
#include "ui/utils.hpp"

class UserInterface {
 public:
  static UserInterface &GetInstance() {
    static UserInterface ui;
    return ui;
  }

  void Start(int argc, char *argv[]);
  void EnterLogin();
  void EnterRegister();
  void EnterMain();

  void CreateChat(std::string);
  void TryUpdateChat(std::string, Message);

  UserInterface(UserInterface const &) = delete;
  void operator=(UserInterface const &) = delete;

 private:
  UserInterface();
  enum Status { LOGIN, REGISTER, MAIN } status;

  Glib::RefPtr<Gtk::Application> app;
  LoginWindow *login_window_;
  RegisterWindow *register_window_;
  ContactWindow *contact_window_;
  std::unordered_map<std::string, ChatWindow *> active_chat_window_;
};

#endif

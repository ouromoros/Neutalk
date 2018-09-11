#include "user_interface.hpp"

#include <assert.h>
#include "client.hpp"
#include "network.hpp"

UserInterface::UserInterface()
    : login_window_(NULL),
      register_window_(NULL),
      contact_window_(NULL),
      status(LOGIN) {}

void UserInterface::Start(int argc, char *argv[]) {
  app = Gtk::Application::create(argc, argv);

  status = LOGIN;
  login_window_ = new LoginWindow();
  app->run(*login_window_);
}

void UserInterface::EnterLogin() {
  assert(login_window_ == NULL);
  login_window_ = new LoginWindow();
  app->add_window(*login_window_);

  if (status == REGISTER) {
    delete register_window_;
    register_window_ = NULL;
  }
  status = LOGIN;
}

void UserInterface::EnterRegister() {
  assert(register_window_ == NULL);
  register_window_ = new RegisterWindow();
  app->add_window(*register_window_);

  if (status == LOGIN) {
    delete login_window_;
    login_window_ = NULL;
  }
  status = REGISTER;
}

void UserInterface::EnterMain() {
  std::vector<std::string> contact;
  Client &client = Client::GetInstance();

  assert(contact_window_ == NULL);
  assert(status == LOGIN);

  // contact = client.RequestContact();
  contact_window_ = new ContactWindow(contact);
  app->add_window(*contact_window_);

  delete login_window_;
  login_window_ = NULL;

  status = MAIN;
}

void UserInterface::CreateChat(std::string username) {
  bool already_active;
  ChatWindow *new_chat_window;

  assert(status == MAIN);

  already_active =
      active_chat_window_.find(username) != active_chat_window_.end();
  if (already_active) {
    // notify
    return;
  }

  new_chat_window = new ChatWindow(username);
  app->add_window(*new_chat_window);

  active_chat_window_[username] = new_chat_window;
}

void UserInterface::TryUpdateChat(std::string username, Message message) {
  bool active;

  assert(status == MAIN);

  active = active_chat_window_.find(username) != active_chat_window_.end();
  if (!active) {
    return;
  }

  active_chat_window_[username]->Update(message);
}

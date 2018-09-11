#include "login_window.hpp"

#include <string>

#include "../client.hpp"
#include "../user_interface.hpp"
#include "utils.hpp"

LoginWindow::LoginWindow()
    : m_button_confirm_("login"),
      m_button_register_("register"),
      m_name_label_("username:"),
      m_pass_label_("password:") {
  m_box_name_.pack_start(m_name_label_);
  m_box_name_.pack_start(m_entry_name_);

  m_box_pass_.pack_start(m_pass_label_);
  m_box_pass_.pack_start(m_entry_pass_);

  add(m_box_name_);
  add(m_box_pass_);
  add(m_button_confirm_);
  add(m_button_register_);

  show_all();

  m_button_confirm_.signal_clicked().connect(
      sigc::mem_fun(*this, &LoginWindow::OnConfirm));
  m_button_register_.signal_clicked().connect(
      sigc::mem_fun(*this, &LoginWindow::OnRegister));
}

void LoginWindow::OnConfirm() {
  bool success;
  std::string username, password;

  username = m_entry_name_.get_text();
  password = m_entry_pass_.get_text();

  if (CheckFormat(username) && CheckFormat(password)) {
    Client &client = Client::GetInstance();
    success = client.TryStart(username, password);
    if (success) {
      ShowSimpleDialog(*this, "Login Success!");
      UserInterface &user_interface = UserInterface::GetInstance();
      user_interface.EnterMain();
    } else {
      ShowSimpleDialog(*this, "Login Fail!");
    }
  } else {
    ShowSimpleDialog(*this, "Bad username/password format!");
  }
}

void LoginWindow::OnRegister() {
  UserInterface &user_interface = UserInterface::GetInstance();
  user_interface.EnterRegister();
}

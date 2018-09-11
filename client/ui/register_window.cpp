#include "register_window.hpp"
#include <string>
#include "../client.hpp"
#include "../user_interface.hpp"
#include "utils.hpp"

RegisterWindow::RegisterWindow()
    : m_button_confirm_("Confirm"),
      m_button_cancel_("Cancel"),
      m_label_confirm_pass_("repeat:"),
      m_label_name_("username:"),
      m_label_pass_("password:") {
  m_box_name_.pack_start(m_label_name_);
  m_box_name_.pack_start(m_entry_name_);

  m_box_pass_.pack_start(m_label_pass_);
  m_box_pass_.pack_start(m_entry_pass_);

  m_box_confirm_pass_.pack_start(m_label_confirm_pass_);
  m_box_confirm_pass_.pack_start(m_entry_confirm_pass_);

  add(m_box_name_);
  add(m_box_pass_);
  add(m_box_confirm_pass_);
  add(m_button_confirm_);
  add(m_button_cancel_);

  show_all();

  m_button_confirm_.signal_clicked().connect(
      sigc::mem_fun(*this, &RegisterWindow::OnConfirm));
  m_button_cancel_.signal_clicked().connect(
      sigc::mem_fun(*this, &RegisterWindow::OnCancel));
}

void RegisterWindow::OnConfirm() {
  std::string username, password, confirm_password;
  bool success;

  username = m_entry_name_.get_text();
  password = m_entry_pass_.get_text();
  confirm_password = m_entry_confirm_pass_.get_text();

  if (!CheckFormat(username) || CheckFormat(password)) {
    ShowSimpleDialog(*this, "Bad username/password format!");
    return;
  }

  if (password != confirm_password) {
    ShowSimpleDialog(*this, "Password not the same!");
    return;
  }

  Client &client = Client::GetInstance();
  success = client.RequestRegister(username, password);
  if (success) {
    ShowSimpleDialog(*this, "Register Success!");
  } else {
    ShowSimpleDialog(*this, "Register Fail!");
  }
}

void RegisterWindow::OnCancel() {
  UserInterface &user_interface = UserInterface::GetInstance();
  user_interface.EnterLogin();
}

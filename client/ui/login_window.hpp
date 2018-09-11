#ifndef CLIENT_UI_LOGIN_WINDOW
#define CLIENT_UI_LOGIN_WINDOW
#include <gtkmm.h>

class LoginWindow : public Gtk::Window {
 public:
  LoginWindow();

 private:
  Gtk::Button m_button_confirm_, m_button_register_;
  Gtk::Label m_name_label_, m_pass_label_;
  Gtk::Entry m_entry_name_, m_entry_pass_;
  Gtk::Box m_box_name_, m_box_pass_;

  void OnConfirm();
  void OnRegister();
};
#endif

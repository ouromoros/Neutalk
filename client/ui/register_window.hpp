#ifndef CLIENT_UI_REGISTER_WINDOW_
#define CLIENT_UI_REGISTER_WINDOW_
#include <gtkmm.h>

class RegisterWindow : public Gtk::Window {
 public:
  RegisterWindow();

 private:
  Gtk::Button m_button_confirm_, m_button_cancel_;
  Gtk::Label m_label_name_, m_label_pass_, m_label_confirm_pass_;
  Gtk::Entry m_entry_name_, m_entry_pass_, m_entry_confirm_pass_;
  Gtk::Box m_box_name_, m_box_pass_, m_box_confirm_pass_;

  void OnConfirm();
  void OnCancel();
};
#endif

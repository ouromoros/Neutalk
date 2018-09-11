#ifndef CLIENT_UI_CONTACT_WINDOW_
#define CLIENT_UI_CONTACT_WINDOW_
#include <gtkmm.h>

#include <string>
#include <vector>

class ContactWindow : public Gtk::Window {
 public:
  ContactWindow(std::vector<std::string>);
  void Update(std::vector<std::string>);

 private:
  Gtk::Button m_button_chat_, m_button_delete_, m_button_add_;

  void OnChat();
  void OnAdd();
  void OnDelete();
};
#endif

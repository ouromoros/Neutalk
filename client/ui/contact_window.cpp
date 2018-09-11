#include "contact_window.hpp"
#include <string>
#include "../user_interface.hpp"

ContactWindow::ContactWindow(std::vector<std::string> contacts)
    : m_button_chat_("Chat"),
      m_button_delete_("Delete"),
      m_button_add_("Addd") {
  add(m_button_chat_);
  add(m_button_add_);
  add(m_button_delete_);

  show_all();

  m_button_add_.signal_clicked().connect(
      sigc::mem_fun(*this, &ContactWindow::OnAdd));
}

void ContactWindow::Update(std::vector<std::string> contacts) {}

void ContactWindow::OnAdd() {
  Gtk::MessageDialog dialog(*this, "Talk to who?", false,
                            Gtk::MESSAGE_QUESTION);
  Gtk::Entry m_entry_name;

  dialog.get_vbox()->pack_start(m_entry_name);
  if (dialog.run() == Gtk::RESPONSE_OK) {
    std::string name = m_entry_name.get_text();
    UserInterface &user_interface = UserInterface::GetInstance();
    user_interface.CreateChat(name);
  }
}

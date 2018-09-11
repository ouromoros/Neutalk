#ifndef CLIENT_UI_CHAT_WINDOW_
#define CLIENT_UI_CHAT_WINDOW_
#include <gtkmm.h>

#include "../data_structure/message.hpp"
#include "../data_structure/message_queue.hpp"

class ChatWindow : public Gtk::Window {
 public:
  ChatWindow(std::string);
  void Update(Message);

 private:
  MessageQueue message_queue_;

  Gtk::TextView m_textview_content_;
  Gtk::Entry m_entry_text_;
  Gtk::Button m_button_send_;
  Glib::Dispatcher m_dispatcher_;

  void UpdateFromMessageQueue();
};

#endif

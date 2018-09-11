#include "chat_window.hpp"

ChatWindow::ChatWindow(std::string username) : m_button_send_("Send") {
  set_title(username);

  add(m_textview_content_);
  add(m_entry_text_);
  add(m_button_send_);

  show_all();

  // ambiguity?
  m_dispatcher_.connect(
      sigc::mem_fun(*this, &ChatWindow::UpdateFromMessageQueue));
}

void ChatWindow::Update(Message message) {
  message_queue_.Push(message);
  m_dispatcher_.emit();
}

void ChatWindow::UpdateFromMessageQueue() {
  Message message;
  message = message_queue_.Pop();

  // Update UI
}

#include "utils.hpp"

using std::string;

void ShowSimpleDialog(Gtk::Window& win, string note) {
  Gtk::MessageDialog dialog(win, note);
  dialog.run();
}

bool ShowConfirmDialog(Gtk::Window& win, string question) {
  Gtk::MessageDialog dialog(win, question, false, Gtk::MESSAGE_QUESTION);
  return dialog.run() == Gtk::RESPONSE_OK;
}

bool CheckFormat(const string& s) {
  int len = s.size();
  for (int i = 0; i < len; i++) {
    if (!isalpha(s[i])) return false;
  }
  return true;
}

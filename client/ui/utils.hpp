#ifndef CLIENT_UI_UTILS_
#define CLIENT_UI_UTILS_
#include <gtkmm.h>
#include <string>

void ShowSimpleDialog(Gtk::Window& win, std::string note);
bool ShowConfirmDialog(Gtk::Window& win, std::string note);
bool CheckFormat(const std::string& s);
#endif

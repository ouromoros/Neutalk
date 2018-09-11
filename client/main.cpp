#include "user_interface.hpp"

int main(int argc, char *argv[]) {
  UserInterface &ui = UserInterface::GetInstance();
  ui.Start(argc, argv);
}

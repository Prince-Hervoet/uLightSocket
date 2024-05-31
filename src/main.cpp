#include <iostream>

#include "ini_parser.hpp"

int main() {
  let_me_see::IniParser ini_parser;
  int a = ini_parser.LoadFile(("./test.ini"));
  std::string value = ini_parser.GetValue("section2222", "server_count");
  std::cout << value << std::endl;
  return 0;
}
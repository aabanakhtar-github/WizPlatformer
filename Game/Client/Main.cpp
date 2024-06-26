#include "Game.h"
#include "Netcode/NetUtil.h"
#include "SDL.h"
#include <iostream>

// the one and only, the holy grail, the main function
int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr
        << "Incorrect argument count! usage: \"./Client <server ip> <port>"
        << std::endl;
    return 1;
  }

  std::string server_ip = argv[1];
  std::string port = argv[2];

  std::cout << "Attempting to connect to:" << server_ip << ":" << port
            << std::endl;

  if (!checkIP(server_ip) && !checkPort(port)) {
    std::cerr << "Invalid IP Address, check your input!" << std::endl;
    return 1;
  }

  Client::Game game(server_ip, std::stoi(port));
  game.run();
  return 0;
}

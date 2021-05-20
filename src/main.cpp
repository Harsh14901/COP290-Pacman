#include <MainGame.hpp>
#include <SimGame.hpp>
#include <csignal>
#include <iostream>

#include "Utils/PreferenceManager.hpp"

using namespace std;
bool is_server = false;
bool is_two_player = true;
bool network_error = false;
void signal_handler(int signal_num) {
  cout << "\nOK BYE!" << endl;

  // terminate program
  exit(signal_num);
}

int main(int argc, char* argv[]) {
  PreferenceManager::initSettings();

  signal(SIGINT, signal_handler);
  string host = "";

  if (argc >= 2) {
    host = string(argv[1]);
  }

  Server server;
  Client client(host, PORT);
  if (host == "simulate") {
    SimGame().runSimulation();
  } else {
    MainGame mainGame;
    if (host == "") {
      is_server = true;
      cout << "Is server is true" << endl;
      mainGame.listen(&server);
    } else {
      is_server = false;
      mainGame.connect(&client);
    }
    mainGame.runGame();
  }

  return 0;
}
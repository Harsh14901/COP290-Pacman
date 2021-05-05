#include <MainGame.hpp>
#include <csignal>
#include <iostream>

using namespace std;
bool is_server = false;
bool is_two_player = true;
void signal_handler(int signal_num) {
  cout << "\nOK BYE!" << endl;

  // terminate program
  exit(signal_num);
}

int main(int argc, char* argv[]) {
  signal(SIGINT, signal_handler);
  MainGame mainGame;
  string host = "";

  if (argc >= 2) {
    host = string(argv[1]);
  }

  Server server;
  Client client(host, PORT);

  if (host == "") {
    is_server = true;
    mainGame.listen(&server);
  } else {
    is_server = false;
    mainGame.connect(&client);
  }
  mainGame.runGame();
  return 0;
}
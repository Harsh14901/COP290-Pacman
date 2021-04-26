#include <MainGame.hpp>
#include <csignal>
#include <iostream>

using namespace std;

void signal_handler(int signal_num) {
  cout << "\nOK BYE!"<<endl;

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
    mainGame.listen(&server);
  } else {
    mainGame.connect(&client);
  }
  mainGame.runGame();
  return 0;
}
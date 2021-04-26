#pragma once

#include <SDL2/SDL_net.h>
#include <bits/stdc++.h>
#include "Constants.hpp"

using namespace std;

class Packet{
  public:
    string data;
    Packet();
    void decode(char* buffer);
    void encode(char* buffer);
};

class NetworkDevice{
  public:
    NetworkDevice();
    NetworkDevice(TCPsocket* recv_socket, TCPsocket* send_socket);
    void recv();
    void send(Packet& packet);
    bool packet_ready();
    void get_packet(Packet& packet);
    ~NetworkDevice();
  protected:
    queue<Packet> packet_buffer;
  private:
    const static int MAX_BUFFER = 4096;
    TCPsocket* recv_socket;
    TCPsocket* send_socket;
};

class Server: public NetworkDevice {
 public:
  Server(int port);
  Server();

  void init();
  void wait_for_connection();
  bool is_connected();

  ~Server();

 private:
  int port;
  bool connected;
  IPaddress ip, remote_ip;
  TCPsocket sd, csd;
};

class Client: public NetworkDevice{
  public:
    Client();
    Client(string server_host, int port);
    
    void init();
    void connect();
    
    ~Client();

  private:
    string server_host;
    int port;
    IPaddress server_ip;
    TCPsocket sd, csd;

};
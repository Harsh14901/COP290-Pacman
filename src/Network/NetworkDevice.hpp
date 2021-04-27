#pragma once

#include <SDL2/SDL_net.h>
#include <bits/stdc++.h>

#include "Constants.hpp"
#include "Network/Packet.hpp"
using namespace std;

class NetworkDevice {
 public:
  NetworkDevice();
  NetworkDevice(TCPsocket* recv_socket, TCPsocket* send_socket);
  void recv();
  void send(PacketStore& ps);
  bool packet_ready();
  void get_packets(PacketStore& ps);

 protected:
  queue<PacketStore> ps_buffer;

 private:
  const static int MAX_BUFFER = MAX_BUFF;
  TCPsocket* recv_socket;
  TCPsocket* send_socket;
};

class Server : public NetworkDevice {
 public:
  Server(int port);
  Server();

  void init();
  void wait_for_connection();
  bool is_connected();

 private:
  int port;
  bool connected;
  IPaddress ip, remote_ip;
  TCPsocket sd, csd;
};

class Client : public NetworkDevice {
 public:
  Client();
  Client(string server_host, int port);

  void init();
  void connect();

 private:
  string server_host;
  int port;
  IPaddress server_ip;
  TCPsocket sd;
};
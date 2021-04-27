#pragma once

#include <SDL2/SDL_net.h>
#include <bits/stdc++.h>
#include "Constants.hpp"

using namespace std;

class Packet{
  public:
    string id;
    string data;
    Packet();
    void decode(char* buffer);
    void encode(char* buffer);
  private:
    char delimiter;
};

class NetworkDevice{
  public:
    NetworkDevice();
    NetworkDevice(TCPsocket* recv_socket, TCPsocket* send_socket);
    void recv();
    void send(Packet& packet);
    bool packet_ready();
    void get_packet(Packet& packet);
  protected:
    queue<Packet> packet_buffer;
  private:
    const static int MAX_BUFFER = 4096;
    TCPsocket* recv_socket;
    TCPsocket* send_socket;
};


class NetworkManager{
  public:
    static void get_packets(string id, vector<Packet>& packets);
    static void load_device(NetworkDevice* device);
    static void recv_packets();
    static void send_packet(Packet& packet);
  private:
    static NetworkDevice* device;
    static unordered_map<string, vector<Packet>> packet_store;
};

class Server: public NetworkDevice {
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

class Client: public NetworkDevice{
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
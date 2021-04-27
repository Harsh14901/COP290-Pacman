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

class PacketStore{
  public:
    PacketStore();
    void add_packet(Packet& packet);
    void get_packets(vector<Packet>& packets);
    void encode(char* buffer);
    void decode(char* buffer);
    void clear();
  private:
    vector<Packet> packets;
    char delimiter;

};

class NetworkDevice{
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


class NetworkManager{
  public:
    static void get_packets(string id, vector<Packet>& packets);
    static void load_device(NetworkDevice* device);
    static void recv_packets();
    static void send_packets();
    static void queue_packet(Packet& packet);
  private:
    static NetworkDevice* device;
    static unordered_map<string, vector<Packet>> packet_map;
    static PacketStore tosend_packets;

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
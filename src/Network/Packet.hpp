#pragma once

#include <SDL2/SDL_net.h>
#include <bits/stdc++.h>

#include "Constants.hpp"

using namespace std;

class Packet {
 public:
  string id;
  string data;
  Packet();
  void decode(char* buffer);
  void encode(char* buffer);

 private:
  char delimiter;
};

class PacketStore {
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
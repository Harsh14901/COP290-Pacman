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

  // Returns the number of packets encoded in the buffer
  // The buffer size is constrainted by MAX_BUFF in constants.hpp
  int encode(char* buffer);
  void decode(char* buffer);
  void clear(int num);

 private:
  vector<Packet> packets;
  char delimiter;
};
#pragma once

#include <SDL2/SDL_net.h>
#include <bits/stdc++.h>

#include "Constants.hpp"

using namespace std;

class Packet {
 public:
  string id = "";
  int posX = 0;
  int posY = 0;
  int velX = 0;
  int velY = 0;
  string data = "";
  Packet();
  Packet(string id, string data);
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
  int size();
  // Returns the number of packets encoded in the buffer
  // The buffer size is constrainted by MAX_BUFF in constants.hpp
  int encode(char* buffer);
  void decode(char* buffer);
  void clear(int num);
  vector<Packet> packets;

 private:
  char delimiter;
};

extern Packet ack;
void check_ack(vector<Packet>& packets);
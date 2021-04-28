#pragma once

#include <SDL2/SDL_net.h>
#include <bits/stdc++.h>

#include "Constants.hpp"
#include "Network/NetworkDevice.hpp"
#include "Network/Packet.hpp"

using namespace std;

class NetworkManager {
 public:
  static void get_packets(string id, vector<Packet>& packets);
  static void load_device(NetworkDevice* device);
  static void recv_packets();

  // Returns the number of packets sent through network manager
  static int send_packets();
  static void queue_packet(Packet& packet);

 private:
  static NetworkDevice* device;
  static unordered_map<string, vector<Packet>> packet_map;
  static PacketStore tosend_packets;
};

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

  // Returns the number of packets recieved
  static int recv_packets(bool to_clear = true);

  // Returns the number of packets sent through network manager
  static int send_packets();

  // Queue the packet for sending;
  static void queue_packet(Packet& packet);

  // Send all the packets in packet store using some protocol
  static void send_all();

  // Recieve all the packets that were sent by send all using the protocol
  static void recv_all();

  // Send a single ack packet
  static void send_ack();

  // Recieve a single ack packet
  static void recv_ack();

 private:
  static NetworkDevice* device;
  static unordered_map<string, vector<Packet>> packet_map;
  static PacketStore tosend_packets;
  static string NET_ID;
};

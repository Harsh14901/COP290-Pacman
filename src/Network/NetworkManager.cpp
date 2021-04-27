#include "Network/NetworkManager.hpp"

NetworkDevice* NetworkManager::device;
unordered_map<string, vector<Packet>> NetworkManager::packet_map;
PacketStore NetworkManager::tosend_packets;

void NetworkManager::get_packets(string id, vector<Packet>& packets) {
  packets = packet_map[id];
}

void NetworkManager::load_device(NetworkDevice* device) {
  NetworkManager::device = device;
}

void NetworkManager::recv_packets() {
  if (device == nullptr) {
    fatalError("Cannot recieve packets from a NULL device");
    return;
  }
  packet_map.clear();
  device->recv();
  while (device->packet_ready()) {
    cout << "NetworkManager::recv_packets: PacketStore is ready " << endl;
    PacketStore ps;
    device->get_packets(ps);
    vector<Packet> packets;
    ps.get_packets(packets);
    for (auto& p : packets) {
      packet_map[p.id].push_back(p);
    }
  }
}

void NetworkManager::queue_packet(Packet& packet) {
  tosend_packets.add_packet(packet);
}

void NetworkManager::send_packets() {
  if (device == nullptr) {
    fatalError("Cannot send packets from a NULL device");
    return;
  }
  device->send(tosend_packets);
  tosend_packets.clear();
}
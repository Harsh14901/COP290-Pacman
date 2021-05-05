#include "Network/NetworkManager.hpp"

NetworkDevice* NetworkManager::device;
unordered_map<string, vector<Packet>> NetworkManager::packet_map;
PacketStore NetworkManager::tosend_packets;
string NetworkManager::NET_ID = NETWORK_ID;

void NetworkManager::get_packets(string id, vector<Packet>& packets) {
  packets = packet_map[id];
}

void NetworkManager::load_device(NetworkDevice* device) {
  NetworkManager::device = device;
}

int NetworkManager::recv_packets(bool to_clear) {
  if (device == nullptr) {
    fatalError("Cannot recieve packets from a NULL device");
    return 0;
  }
  if (to_clear) {
    packet_map.clear();
  }
  device->recv();
  int num = 0;
  while (device->packet_ready()) {
    // cout << "NetworkManager::recv_packets: PacketStore is ready " << endl;

    PacketStore ps;
    device->get_packets(ps);
    vector<Packet> packets;
    ps.get_packets(packets);
    for (auto& p : packets) {
      packet_map[p.id].push_back(p);
      num++;
    }
  }
  return num;
}

void NetworkManager::queue_packet(Packet& packet) {
  tosend_packets.add_packet(packet);
}

void NetworkManager::clear_all(){
  tosend_packets.clear(tosend_packets.size());
}

int NetworkManager::send_packets() {
  if (device == nullptr) {
    fatalError("Cannot send packets from a NULL device");
    return -1;
  }
  int num = device->send(tosend_packets);
  tosend_packets.clear(num);
  return num;
}

void NetworkManager::send_all() {
  int num = tosend_packets.size();
  PacketStore ps = tosend_packets;
  tosend_packets.clear(num);

  Packet p;
  p.id = NET_ID;
  p.data = to_string(num);

  queue_packet(p);
  send_packets();
  recv_ack();

  tosend_packets = ps;
  int n = 0;
  while (n != num) {
    n += send_packets();
    recv_ack();
  }
}

void NetworkManager::recv_all() {
  recv_packets();
  vector<Packet> packets;
  get_packets(NET_ID, packets);
  assert(packets.size() == 1 && packets[0].id == NET_ID);
  send_ack();

  int num = stoi(packets[0].data);
  int n = 0;
  while (n != num) {
    n += recv_packets(false);
    send_ack();
  }
}

void NetworkManager::recv_ack() {
  int n = recv_packets();
  assert(n == 1);

  vector<Packet> packets;
  get_packets(ack.id, packets);

  check_ack(packets);
}

void NetworkManager::send_ack() {
  queue_packet(ack);
  int n = send_packets();
  assert(n == 1);
}
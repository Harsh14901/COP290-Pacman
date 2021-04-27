#include "Network/Packet.hpp"

Packet::Packet() : delimiter('|') {}

void Packet::encode(char* buffer) {
  auto combined = id + delimiter + data;
  auto encoded = combined.c_str();
  strcpy(buffer, encoded);
}

void Packet::decode(char* buffer) {
  string buff = string(buffer);
  stringstream parse_stream(buff);
  string decoded;
  vector<string> tokens;
  while (getline(parse_stream, decoded, delimiter)) {
    tokens.push_back(decoded);
  }

  id = tokens[0];
  data = tokens[1];
}

PacketStore::PacketStore() : packets(vector<Packet>()), delimiter(';') {}
void PacketStore::add_packet(Packet& packet) { packets.push_back(packet); }

void PacketStore::get_packets(vector<Packet>& p) { p = packets; }

void PacketStore::encode(char* buffer) {
  string encoded = to_string(packets.size()) + delimiter;
  for (auto& p : packets) {
    char buffer[MAX_BUFF];
    p.encode(buffer);
    encoded += string(buffer);
    encoded += delimiter;
  }
  strcpy(buffer, encoded.c_str());
}
void PacketStore::decode(char* buffer) {
  string buff = string(buffer);
  stringstream parse_stream(buff);
  string decoded;
  vector<string> tokens;
  while (getline(parse_stream, decoded, delimiter)) {
    tokens.push_back(decoded);
  }
  int n = stoi(tokens[0]);
  for (int i = 1; i <= n; i++) {
    Packet p;
    p.decode(strdup(tokens[i].c_str()));
    packets.push_back(p);
  }
}

void PacketStore::clear() { packets.clear(); }

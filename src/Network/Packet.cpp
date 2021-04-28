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

int PacketStore::encode(char* buffer) {
  string encoded = "";
  int n = 0;
  int margin = (to_string(MAX_BUFF)).size() + 1;
  for (auto& p : packets) {
    char buffer[MAX_BUFF];
    p.encode(buffer);
    auto p_enc = string(buffer) + delimiter;
    if (encoded.size() + p_enc.size() + margin > MAX_BUFF) {
      break;
    }
    encoded += p_enc;
    n++;
  }
  encoded = to_string(n) + delimiter + encoded;
  // cout<< "Encoded packet: "<<encoded<<endl;
  strcpy(buffer, encoded.c_str());
  return n;
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
  assert(int(tokens.size()) == n + 1);
  for (int i = 1; i <= n; i++) {
    Packet p;
    p.decode(strdup(tokens[i].c_str()));
    packets.push_back(p);
  }
}

void PacketStore::clear(int num) {
  packets.erase(packets.begin(), packets.begin() + num);
}

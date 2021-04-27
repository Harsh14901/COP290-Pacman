#include "NetworkManager.hpp"

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

PacketStore::PacketStore(): packets(vector<Packet>()), delimiter(';'){}
void PacketStore::add_packet(Packet& packet){
  packets.push_back(packet);
}

void PacketStore::get_packets(vector<Packet>& p){
  p = packets;
}

void PacketStore::encode(char* buffer){
  string encoded = to_string(packets.size()) + delimiter;
  for(auto&p :packets){
    char buffer[MAX_BUFF];
    p.encode(buffer);
    encoded += string(buffer);
    encoded +=  delimiter;
  }
  strcpy(buffer, encoded.c_str());

}
void PacketStore::decode(char* buffer){
  string buff = string(buffer);
  stringstream parse_stream(buff);
  string decoded;
  vector<string> tokens;
  while (getline(parse_stream, decoded, delimiter)) {
    tokens.push_back(decoded);
  }
  int n = stoi(tokens[0]);
  for (int i = 1; i <= n; i++){
    Packet p;
    p.decode(strdup(tokens[i].c_str()));
    packets.push_back(p);
  }
  
}

void PacketStore::clear(){
  packets.clear();
}

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
    cout<<"NetworkManager::recv_packets: PacketStore is ready "<<endl;
    PacketStore ps;
    device->get_packets(ps);
    vector<Packet> packets;
    ps.get_packets(packets);
    for(auto&p : packets){
      packet_map[p.id].push_back(p); 
    }
  }
}

void NetworkManager::queue_packet(Packet& packet) {
  tosend_packets.add_packet(packet);
}

void NetworkManager::send_packets(){
  if (device == nullptr) {
    fatalError("Cannot send packets from a NULL device");
    return;
  }
  device->send(tosend_packets);
  tosend_packets.clear();
}

const int NetworkDevice::MAX_BUFFER;
NetworkDevice::NetworkDevice() {
  recv_socket = nullptr;
  send_socket = nullptr;
  ps_buffer = queue<PacketStore>();
}

NetworkDevice::NetworkDevice(TCPsocket* recv_socket, TCPsocket* send_socket)
    : recv_socket(recv_socket), send_socket(send_socket) {}

bool NetworkDevice::packet_ready() { return !ps_buffer.empty(); }

void NetworkDevice::get_packets(PacketStore& ps) {
  ps = ps_buffer.front();
  ps_buffer.pop();
}

void NetworkDevice::recv() {
  char buffer[MAX_BUFFER];
  cout<<"NetworkDevice::recv() called"<<endl;
  if (SDLNet_TCP_Recv(*recv_socket, buffer, MAX_BUFFER) > 0) {
    printf("packet data recieved: %s\n", buffer);

    auto ps = PacketStore();
    ps.decode(buffer);
    ps_buffer.push(ps);
  }
  printf("buffer size: %ld\n", ps_buffer.size());
}

void NetworkDevice::send(PacketStore& ps) {
  char buffer[MAX_BUFFER];
  ps.encode(buffer);
  int size = strlen(buffer) + 1;
  printf("Sending buffer: %s\n", buffer);
  if (SDLNet_TCP_Send(*send_socket, buffer, size) < size) {
    fatalError("Error sending packet store: " + string(SDLNet_GetError()));
  } else{
    cout<<"packet store sent"<<endl;
  }
}

Server::Server(int port) : NetworkDevice(&csd, &csd), port(port) {}
Server::Server() : Server(PORT) {}

void Server::init() {
  if (SDLNet_ResolveHost(&ip, NULL, port) < 0) {
    fatalError("SDLNet_ResolveHost: " + string(SDLNet_GetError()));
  }

  if (!(sd = SDLNet_TCP_Open(&ip))) {
    fatalError("SDLNet_TCP_Open: " + string(SDLNet_GetError()));
  }
}

void Server::wait_for_connection() {
  if ((csd = SDLNet_TCP_Accept(sd))) {
    IPaddress* remote_ip_ptr;
    if ((remote_ip_ptr = SDLNet_TCP_GetPeerAddress(csd))) {
      remote_ip = *remote_ip_ptr;
      printf("[+] Host connected: %x %d\n", SDLNet_Read32(&(remote_ip.host)),
             SDLNet_Read16(&(remote_ip.port)));
      connected = true;
    } else {
      fatalError("SDLNet_TCP_GetPeerAddress: " + string(SDLNet_GetError()));
    }
  }
}

bool Server::is_connected() { return connected; }

Client::Client(string server_host, int port)
    : NetworkDevice(&sd, &sd), server_host(server_host), port(port) {}
Client::Client() : Client("", PORT) {}

void Client::init() {
  if (SDLNet_ResolveHost(&server_ip, server_host.c_str(), port) < 0) {
    fatalError("SDLNet_ResolveHost:" + string(SDLNet_GetError()));
  }
}

void Client::connect() {
  if (!(sd = SDLNet_TCP_Open(&server_ip))) {
    fatalError("SDLNet_TCP_Open:" + string(SDLNet_GetError()));
  }
}
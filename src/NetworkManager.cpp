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

NetworkDevice* NetworkManager::device;
unordered_map<string, vector<Packet>> NetworkManager::packet_store;

void NetworkManager::get_packets(string id, vector<Packet>& packets) {
  packets = packet_store[id];
}

void NetworkManager::load_device(NetworkDevice* device) {
  NetworkManager::device = device;
}

void NetworkManager::recv_packets() {
  if (device == nullptr) {
    fatalError("Cannot recieve packets from a NULL device");
    return;
  }
  packet_store.clear();
  device->recv();
  while (device->packet_ready()) {
    cout<<"NetworkManager::recv_packets: Packet is ready "<<endl;
    Packet p;
    device->get_packet(p);
    packet_store[p.id].push_back(p);
  }
}

void NetworkManager::send_packet(Packet& packet) {
  if (device == nullptr) {
    fatalError("Cannot send packets from a NULL device");
    return;
  }
  device->send(packet);
}

const int NetworkDevice::MAX_BUFFER;
NetworkDevice::NetworkDevice() {
  recv_socket = nullptr;
  send_socket = nullptr;
  packet_buffer = queue<Packet>();
}

NetworkDevice::NetworkDevice(TCPsocket* recv_socket, TCPsocket* send_socket)
    : recv_socket(recv_socket), send_socket(send_socket) {}

bool NetworkDevice::packet_ready() { return !packet_buffer.empty(); }

void NetworkDevice::get_packet(Packet& packet) {
  packet = packet_buffer.front();
  packet_buffer.pop();
}

void NetworkDevice::recv() {
  char buffer[MAX_BUFFER];
  cout<<"NetworkDevice::recv() called"<<endl;
  if (SDLNet_TCP_Recv(*recv_socket, buffer, MAX_BUFFER) > 0) {
    printf("packet data recieved: %s\n", buffer);

    auto packet = Packet();
    packet.decode(buffer);
    printf("packet id: %s, packet data: %s\n", packet.id.c_str(), packet.data.c_str());
    packet_buffer.push(packet);
  }
  printf("buffer size: %ld\n", packet_buffer.size());
}

void NetworkDevice::send(Packet& packet) {
  char buffer[MAX_BUFFER];
  packet.encode(buffer);
  int size = strlen(buffer) + 1;
  printf("Sending buffer: %s\n", buffer);
  if (SDLNet_TCP_Send(*send_socket, buffer, size) < size) {
    fatalError("Error sending packet: " + string(SDLNet_GetError()));
  } else{
    cout<<"Packet sent"<<endl;
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
#include "NetworkManager.hpp"

Packet::Packet() {}

void Packet::encode(char* buffer) {
  auto encoded = data.c_str();
  strcpy(buffer, encoded);
}

void Packet::decode(char* buffer) {
  auto decoded = string(buffer);
  data = buffer;
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

void NetworkDevice::get_packet(Packet& packet) { return packet_buffer.pop(); }

void NetworkDevice::recv() {
  char buffer[MAX_BUFFER];
  while (SDLNet_TCP_Recv(*recv_socket, buffer, MAX_BUFFER) > 0) {
    auto packet = Packet();
    packet.decode(buffer);
    packet_buffer.push(packet);
  }
}

void NetworkDevice::send(Packet& packet) {
  char buffer[MAX_BUFFER];
  packet.encode(buffer);
  int size = strlen(buffer) + 1;
  if (SDLNet_TCP_Send(*send_socket, buffer, size) < size) {
    fatalError("Error sending packet: " + string(SDLNet_GetError()));
  }
}

NetworkDevice::~NetworkDevice() {
  if (send_socket != nullptr) {
    SDLNet_TCP_Close(*send_socket);
  }
  if (recv_socket != nullptr) {
    SDLNet_TCP_Close(*recv_socket);
  }
}

Server::Server(int port) : NetworkDevice(&csd, &sd), port(port) {}
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

Server::~Server() { printf("Server exiting\n"); }

Client::Client(string server_host, int port)
    : NetworkDevice(&sd, &csd), server_host(server_host), port(port) {}
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

Client::~Client() { printf("Client exiting\n"); }
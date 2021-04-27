#include "Network/NetworkDevice.hpp"

#include "Network/Packet.hpp"

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
  // cout << "NetworkDevice::recv() called" << endl;
  if (SDLNet_TCP_Recv(*recv_socket, buffer, MAX_BUFFER) > 0) {
    // printf("packet data recieved: %s\n", buffer);

    auto ps = PacketStore();
    ps.decode(buffer);
    ps_buffer.push(ps);
  }
  // printf("buffer size: %ld\n", ps_buffer.size());
}

void NetworkDevice::send(PacketStore& ps) {
  char buffer[MAX_BUFFER];
  ps.encode(buffer);
  int size = strlen(buffer) + 1;
  // printf("Sending buffer: %s\n", buffer);
  if (SDLNet_TCP_Send(*send_socket, buffer, size) < size) {
    fatalError("Error sending packet store: " + string(SDLNet_GetError()));
  } else {
    // cout << "packet store sent" << endl;
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
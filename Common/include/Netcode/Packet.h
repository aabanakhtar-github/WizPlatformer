#ifndef PACKET_H
#define PACKET_H

#include "MathFuncs.h"
#include <array>
#include <cstdint>
#include <istream>
#include <string>

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include "enet/enet.h"

enum PacketType {
  PT_GAME_UPDATE,
  PT_CONNECT_UPDATE,
  PT_DISCONNECT_UPDATE,
  PT_HANDSHAKE,
  PT_HANDSHAKE_RESULT,
  PT_CLIENT_JOINED_OR_LEFT,
  PT_CLIENT_JUMP,
  PT_INVALID
};

// TODO: Clarify between network_ID and ECS_ID so my brain can handle it
struct ClientInfo {
  std::size_t network_ID = -1;
  Vector2 position = {0, 0};
  std::string anim_state;
  bool facing_left;
};

struct PacketData {
  int salt = 0;
  std::size_t ID = -1;
  PacketType type = PT_INVALID;
  std::size_t data_size = 0;
  std::string data;
};

// Client side only
struct ConnectDisconnectPayload {
  std::size_t ID = -1;
  std::size_t username_size = 0;
  std::string username_request;
};

struct HandshakeChallengePayload {
  int server_salt = 0;
  int client_salt = 0;
};

// Client side only
struct HandshakeResponsePayload {
  int response = 0;
};

struct HandshakeAcceptRejectPayload {
  bool accepted = false;
  std::size_t new_ID = -1;
};

struct ClientUpdatePayload {
  std::uint16_t input_bits = 0;
};

struct ServerUpdatePayload {
  std::size_t clients_size = 0;
  std::vector<ClientInfo> client_states;
};

template <typename T> std::string payloadToString(const T &payload);
template <typename T> T payloadFromString(const std::string &payload);

std::istream &operator>>(std::istream &in, PacketType &type);
ENetPacket *packetDataToEnetPacket(const PacketData &packet,
                                   ENetPacketFlag flags);
PacketData packetDataFromEnetPacket(const ENetPacket *packet);

#endif // PACKET_H

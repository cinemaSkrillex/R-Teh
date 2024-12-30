#ifndef RTYPESERVERPROTOCOL_HPP_
#define RTYPESERVERPROTOCOL_HPP_

#include <array>
#include <vector>

namespace RTypeProtocol {

// Enum for message types
enum MessageType : int {
    NEW_CLIENT    = 0x01,
    PLAYER_MOVE   = 0x02,
    EVENT_MESSAGE = 0x03,
    SYNCHRONIZE   = 0x04
};

// Base message structure
struct BaseMessage {
    int  message_type;
    long uuid;
};

// Player move message
struct PlayerMoveMessage : BaseMessage {
    float x;
    float y;
    float step;
    long  timestamp;
};

// Synchronize message
struct SynchronizeMessage : BaseMessage {
    long              timestamp;
    float             x;
    float             y;
    std::vector<long> player_uuids;  // List of currently active player UUIDs
};

// Event message structure
struct EventMessage : BaseMessage {
    std::vector<std::pair<int, std::vector<char>>> components;  // Component-based payload
};

// Unified serialize/deserialize functions with templated buffer size
template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const BaseMessage& msg);

template <std::size_t BUFFER_SIZE>
BaseMessage deserialize(const std::array<char, BUFFER_SIZE>& buffer);

// Helper functions for specific message types
template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const PlayerMoveMessage& msg);

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const EventMessage& msg);

template <std::size_t BUFFER_SIZE>
PlayerMoveMessage deserializePlayerMove(const std::array<char, BUFFER_SIZE>& buffer);

template <std::size_t BUFFER_SIZE>
EventMessage deserializeEventMessage(const std::array<char, BUFFER_SIZE>& buffer);

}  // namespace RTypeProtocol

#include "RtypeServerProtocol.tpp"
#endif /* !RTYPESERVERPROTOCOL_HPP_ */

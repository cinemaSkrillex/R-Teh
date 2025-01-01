#ifndef RTYPESERVERPROTOCOL_HPP_
#define RTYPESERVERPROTOCOL_HPP_

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <vector>

namespace RTypeProtocol {

// Enum for message types
enum MessageType : int {
    NEW_CLIENT       = 0x01,
    PLAYER_MOVE      = 0x02,
    EVENT_MESSAGE    = 0x03,
    SYNCHRONIZE      = 0x04,
    PLAYER_DIRECTION = 0x05,
};

// Base message structure (common across all message types)
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

// Player direction message
struct PlayerDirectionMessage : BaseMessage {
    sf::IntRect direction;
    long        timestamp;
};

// Synchronize message
struct SynchronizeMessage : BaseMessage {
    long                                       timestamp;
    float                                      x;
    float                                      y;
    std::vector<std::pair<long, sf::Vector2f>> players;
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

// Helper functions for serializing specific message types
template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const PlayerMoveMessage& msg);

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const EventMessage& msg);

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const SynchronizeMessage& msg);

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const PlayerDirectionMessage& msg);

// Helper function to deserialize different message types
template <std::size_t BUFFER_SIZE>
PlayerMoveMessage deserializePlayerMove(const std::array<char, BUFFER_SIZE>& buffer);

template <std::size_t BUFFER_SIZE>
EventMessage deserializeEventMessage(const std::array<char, BUFFER_SIZE>& buffer);

template <std::size_t BUFFER_SIZE>
PlayerDirectionMessage deserializePlayerDirection(const std::array<char, BUFFER_SIZE>& buffer);

template <std::size_t BUFFER_SIZE>
SynchronizeMessage deserializeSynchronize(const std::array<char, BUFFER_SIZE>& buffer);

}  // namespace RTypeProtocol

#include "RtypeServerProtocol.tpp"
#endif /* !RTYPESERVERPROTOCOL_HPP_ */

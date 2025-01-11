#ifndef RTYPESERVERPROTOCOL_HPP_
#define RTYPESERVERPROTOCOL_HPP_

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <string>
#include <vector>

#include "../ECS/Components/Collision.hpp"

namespace RTypeProtocol {

// Enum for message types
enum MessageType : int {
    NEW_CLIENT          = 0x01,
    PLAYER_MOVE         = 0x02,
    NEW_ENTITY          = 0x03,
    SYNCHRONISE         = 0x04,
    PLAYER_DIRECTION    = 0x05,
    DESTROY_ENTITY      = 0x06,
    SHOOT_EVENT         = 0x07,
    HOLD_SHOOT_EVENT    = 0x08,
    RELEASE_SHOOT_EVENT = 0x09,
    MAP_INFO            = 0x0A,
};

enum ComponentList : int {
    POSITION          = 0x01,
    VELOCITY          = 0x02,
    ROTATION          = 0x03,
    COLLISION         = 0x04,
    AUTO_DESTRUCTIBLE = 0x05,
    DRAWABLE          = 0x06,
    SPRITE            = 0x07,
    BLOCKTAG          = 0x08,
    // SPRITESHEET       = 0x08,
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
struct NewEntityMessage : BaseMessage {
    std::vector<std::pair<ComponentList, std::vector<char>>>
        components;  // Component ID and serialized data
};

struct DestroyEntityMessage : BaseMessage {
    std::vector<long> entity_ids;
};

struct MapMessage : BaseMessage {
    float scrollingSpeed;
    float x_level_position;
};

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const BaseMessage& msg);

template <std::size_t BUFFER_SIZE>
BaseMessage deserialize(const std::array<char, BUFFER_SIZE>& buffer);

// Helper functions for serializing specific message types
template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const PlayerMoveMessage& msg);

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const NewEntityMessage& msg);

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const SynchronizeMessage& msg);

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const PlayerDirectionMessage& msg);

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const DestroyEntityMessage& msg);

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const MapMessage& msg);

// Helper function to deserialize different message types
template <std::size_t BUFFER_SIZE>
PlayerMoveMessage deserializePlayerMove(const std::array<char, BUFFER_SIZE>& buffer);

template <std::size_t BUFFER_SIZE>
NewEntityMessage deserializeNewEntityMessage(const std::array<char, BUFFER_SIZE>& buffer);

template <std::size_t BUFFER_SIZE>
PlayerDirectionMessage deserializePlayerDirection(const std::array<char, BUFFER_SIZE>& buffer);

template <std::size_t BUFFER_SIZE>
SynchronizeMessage deserializeSynchronize(const std::array<char, BUFFER_SIZE>& buffer);

template <std::size_t BUFFER_SIZE>
DestroyEntityMessage deserializeDestroyEntity(const std::array<char, BUFFER_SIZE>& buffer);

template <std::size_t BUFFER_SIZE>
MapMessage deserializeMapMessage(const std::array<char, BUFFER_SIZE>& buffer);

}  // namespace RTypeProtocol

#include "RtypeServerProtocol.tpp"

#endif /* !RTYPESERVERPROTOCOL_HPP_ */

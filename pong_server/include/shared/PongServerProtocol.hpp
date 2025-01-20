/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongServerProtocol.hpp
*/

#ifndef PONGSERVERPROTOCOL_HPP_
#define PONGSERVERPROTOCOL_HPP_

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <string>
#include <vector>

#include "../ECS/Components/Collision.hpp"
#include "../ECS/Components/Position.hpp"
#include "../ECS/Components/Velocity.hpp"

namespace PongProtocol {

// Enum for message types
enum MessageType : int {
    NEW_CLIENT         = 0x01,
    PLAYER_MOVE        = 0x02,
    NEW_ENTITY         = 0x03,
    PLAYER_DIRECTION   = 0x04,
    DESTROY_ENTITY     = 0x05,
    ENTITY_UPDATE      = 0x06,
    PLAYER_UPDATE_DATA = 0x07,
    PLAYER_CONTROL     = 0x08,
    PLAYER_READY       = 0x09,
};

enum ComponentList : int {
    POSITION          = 0x01,
    VELOCITY          = 0x02,
    ROTATION          = 0x03,
    COLLISION         = 0x04,
    AUTO_DESTRUCTIBLE = 0x05,
    DRAWABLE          = 0x06,
    SPRITE            = 0x07,
    ANGLE             = 0x08,
    ACCELERATION      = 0x09,
    INTERPOLATION     = 0x0A,
    CONTROLABLE       = 0x0B,
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

// Event message structure
struct NewEntityMessage : BaseMessage {
    std::vector<std::pair<ComponentList, std::vector<char>>>
        components;  // Component ID and serialized data
};

struct DestroyEntityMessage : BaseMessage {
    std::vector<long> entity_ids;
};

// Entity update, position, rotation(angle) - should be enough for now
struct EntityUpdateMessage : BaseMessage {
    float x;
    float y;
    float angle;
    float step;
    long  timestamp;
};

struct PlayerUpdateDataMessage : BaseMessage {
    int score;
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
std::array<char, BUFFER_SIZE> serialize(const PlayerDirectionMessage& msg);

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const EntityUpdateMessage& msg);

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const PlayerUpdateDataMessage& msg);

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const DestroyEntityMessage& msg);


// Helper function to deserialize different message types
template <std::size_t BUFFER_SIZE>
PlayerMoveMessage deserializePlayerMove(const std::array<char, BUFFER_SIZE>& buffer);

template <std::size_t BUFFER_SIZE>
NewEntityMessage deserializeNewEntity(const std::array<char, BUFFER_SIZE>& buffer);

template <std::size_t BUFFER_SIZE>
PlayerDirectionMessage deserializePlayerDirection(const std::array<char, BUFFER_SIZE>& buffer);

template <std::size_t BUFFER_SIZE>
EntityUpdateMessage deserializeEntityUpdate(const std::array<char, BUFFER_SIZE>& buffer);

template <std::size_t BUFFER_SIZE>
PlayerUpdateDataMessage deserializePlayerUpdateData(const std::array<char, BUFFER_SIZE>& buffer);

template <std::size_t BUFFER_SIZE>
DestroyEntityMessage deserializeDestroyEntity(const std::array<char, BUFFER_SIZE>& buffer);

}  // namespace PongProtocol

#include "PongServerProtocol.tpp"

#endif /* !RTYPESERVERPROTOCOL_HPP_ */

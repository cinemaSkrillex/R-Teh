#ifndef RTYPESERVERPROTOCOL_HPP_
#define RTYPESERVERPROTOCOL_HPP_

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <string>
#include <vector>

#include "../ECS/Components/Collision.hpp"
#include "../ECS/Components/Position.hpp"
#include "../ECS/Components/Velocity.hpp"

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
    LEVEL_SIGNAL        = 0x0B,
    ENTITY_UPDATE       = 0x0C,
    // PLAYER_UPDATE_SCORE  = 0x0D,
    // PLAYER_UPDATE_HEALTH = 0x0E,
    PLAYER_UPDATE_DATA = 0x0D,
    MAP_UNLOADED       = 0x0E,
    CHANGING_SCENE     = 0x0F,
    PING               = 0x10,
    PONG              = 0x11,
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
};

enum EntityType : int {
    PLAYER       = 0x01,
    BLOCK        = 0x02,
    OTHER_ENTITY = 0x03,
    BACKGROUND   = 0x04,
};

enum class SceneType { WAITING, MENU, GAME };

struct BackgroundData {
    int   background_id;
    float speed;
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
    EntityType entity_type;
};

struct DestroyEntityMessage : BaseMessage {
    std::vector<long> entity_ids;
};

struct MapMessage : BaseMessage {
    float                       scrollingSpeed;
    float                       x_level_position;
    bool                        isLoaded;
    bool                        isLevelRunning;
    int                         server_tick;
    int                         id_level_music;
    std::vector<BackgroundData> backgrounds;
};

struct LevelSignalMessage : BaseMessage {
    bool startLevel;
};

// Entity update, position, rotation(angle) - should be enough for now
struct EntityUpdateMessage : BaseMessage {
    float x;
    float y;
    float angle;
    float step;
    long  timestamp;
};

struct OneIntMessage : BaseMessage {
    int value;  // can be used for any integer value (e.g. score, health, etc.)
};

struct PlayerUpdateDataMessage : BaseMessage {
    int score;
    int health;
};

struct ChangingSceneMessage : BaseMessage {
    SceneType scene_id;
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

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const LevelSignalMessage& msg);

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const EntityUpdateMessage& msg);

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const OneIntMessage& msg);

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const PlayerUpdateDataMessage& msg);

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> serialize(const ChangingSceneMessage& msg);

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

template <std::size_t BUFFER_SIZE>
LevelSignalMessage deserializeLevelSignal(const std::array<char, BUFFER_SIZE>& buffer);

template <std::size_t BUFFER_SIZE>
EntityUpdateMessage deserializeEntityUpdate(const std::array<char, BUFFER_SIZE>& buffer);

template <std::size_t BUFFER_SIZE>
OneIntMessage deserializeOneIntMessage(const std::array<char, BUFFER_SIZE>& buffer);

template <std::size_t BUFFER_SIZE>
PlayerUpdateDataMessage deserializePlayerUpdateDataMessage(
    const std::array<char, BUFFER_SIZE>& buffer);

template <std::size_t BUFFER_SIZE>
ChangingSceneMessage deserializeChangingSceneMessage(const std::array<char, BUFFER_SIZE>& buffer);


}  // namespace RTypeProtocol

#include "RtypeServerProtocol.tpp"

#endif /* !RTYPESERVERPROTOCOL_HPP_ */

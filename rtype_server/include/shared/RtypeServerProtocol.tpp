/*
** EPITECH PROJECT, 2024
** r-teh-mega-backup
** File description:
** RtypeServerProtocol
*/

#ifndef RTYPESERVERPROTOCOL_TPP_
#define RTYPESERVERPROTOCOL_TPP_

#include <cstring>
#include <iostream>

#include "RtypeServerProtocol.hpp"

// Helper: write any POD type to buffer and advance pointer
template <typename T>
inline void writeToBuffer(char*& dest, const T& value) {
    std::memcpy(dest, &value, sizeof(T));
    dest += sizeof(T);
}

// Helper: read any POD type from buffer and advance pointer
template <typename T>
inline void readFromBuffer(const char*& src, T& value) {
    std::memcpy(&value, src, sizeof(T));
    src += sizeof(T);
}

template <std::size_t BUFFER_SIZE, typename T>
std::array<char, BUFFER_SIZE> serializeBaseMessage(const T& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};
    char*                         it     = buffer.data();
    writeToBuffer(it, msg);
    return buffer;
}

// Deserialize a BaseMessage from the buffer
template <std::size_t BUFFER_SIZE, typename T>
T deserializeBaseMessage(const std::array<char, BUFFER_SIZE>& buffer) {
    T           msg;
    const char* it = buffer.data();
    readFromBuffer(it, msg);
    return msg;
}

// Helper function to serialize a PlayerMoveMessage
template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> RTypeProtocol::serialize(const PlayerMoveMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};
    char*                         it     = buffer.data();

    // Serialize the base message
    writeToBuffer(it, static_cast<const BaseMessage&>(msg));

    // Serialize the additional fields
    writeToBuffer(it, msg.x);
    writeToBuffer(it, msg.y);
    writeToBuffer(it, msg.step);
    writeToBuffer(it, msg.timestamp);

    return buffer;
}

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> RTypeProtocol::serialize(const PlayerDirectionMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};
    char*                         it     = buffer.data();

    // Serialize the base message
    writeToBuffer(it, static_cast<const BaseMessage&>(msg));

    // Serialize the additional fields
    writeToBuffer(it, msg.direction);
    writeToBuffer(it, msg.timestamp);

    return buffer;
}

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> RTypeProtocol::serialize(const DestroyEntityMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};
    char*                         it     = buffer.data();

    // Serialize the base message
    writeToBuffer(it, static_cast<const BaseMessage&>(msg));

    // Serialize the additional fields
    int entity_count = static_cast<int>(msg.entity_ids.size());
    writeToBuffer(it, entity_count);

    for (const auto& entity_id : msg.entity_ids) {
        writeToBuffer(it, entity_id);
    }

    return buffer;
}

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> RTypeProtocol::serialize(const MapMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};
    char*                         offset = buffer.data();

    // Serialize base message
    writeToBuffer(offset, static_cast<const BaseMessage&>(msg));

    // Serialize other fields
    writeToBuffer(offset, msg.scrollingSpeed);
    writeToBuffer(offset, msg.x_level_position);
    writeToBuffer(offset, msg.isLoaded);
    writeToBuffer(offset, msg.isLevelRunning);
    writeToBuffer(offset, msg.server_tick);
    writeToBuffer(offset, msg.id_level_music);

    // Serialize backgrounds vector size
    size_t backgrounds_size = msg.backgrounds.size();
    writeToBuffer(offset, backgrounds_size);

    // Serialize each background
    for (const auto& background : msg.backgrounds) {
        // Serialize background ID
        writeToBuffer(offset, background.background_id);

        // Serialize background speed
        writeToBuffer(offset, background.speed);
    }

    return buffer;
}

template <std::size_t BUFFER_SIZE>
RTypeProtocol::MapMessage RTypeProtocol::deserializeMapMessage(
    const std::array<char, BUFFER_SIZE>& buffer) {
    MapMessage  msg;
    const char* offset = buffer.data();

    readFromBuffer(offset, static_cast<BaseMessage&>(msg));

    // Deserialize other fields
    readFromBuffer(offset, msg.scrollingSpeed);
    readFromBuffer(offset, msg.x_level_position);
    readFromBuffer(offset, msg.isLoaded);
    readFromBuffer(offset, msg.isLevelRunning);
    readFromBuffer(offset, msg.server_tick);
    readFromBuffer(offset, msg.id_level_music);

    size_t backgrounds_size;
    readFromBuffer(offset, backgrounds_size);

    for (size_t i = 0; i < backgrounds_size; ++i) {
        BackgroundData background;

        readFromBuffer(offset, background.background_id);

        readFromBuffer(offset, background.speed);

        msg.backgrounds.push_back(background);
    }

    return msg;
}

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> RTypeProtocol::serialize(const LevelSignalMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};
    std::size_t                   offset = 0;

    // Serialize base message
    std::memcpy(buffer.data() + offset, &msg, sizeof(BaseMessage));
    offset += sizeof(BaseMessage);

    // Serialize startLevel
    std::memcpy(buffer.data() + offset, &msg.startLevel, sizeof(msg.startLevel));
    offset += sizeof(msg.startLevel);

    return buffer;
}

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> RTypeProtocol::serialize(const NewEntityMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};
    char*                         it     = buffer.data();

    // Serialize the base message
    writeToBuffer(it, static_cast<const BaseMessage&>(msg));

    //write the entity type
    writeToBuffer(it, msg.entity_type);

    // Serialize the components
    for (const auto& component : msg.components) {
        // Store the component type as an integer
        writeToBuffer(it, static_cast<int>(component.first));
        // Store the length of the component data
        int dataLength = static_cast<int>(component.second.size());
        writeToBuffer(it, dataLength);
        // Copy component data (variable size)
        std::memcpy(it, component.second.data(), component.second.size());
        it += component.second.size();
    }

    return buffer;
}

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> RTypeProtocol::serialize(const SynchronizeMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};
    char*                         it     = buffer.data();

    // Serialize base message fields
    writeToBuffer(it, msg.message_type);
    writeToBuffer(it, msg.uuid);

    // Serialize specific fields
    writeToBuffer(it, msg.timestamp);
    writeToBuffer(it, msg.x);
    writeToBuffer(it, msg.y);

    // Serialize player UUID and position list
    int player_count = static_cast<int>(msg.players.size());
    writeToBuffer(it, player_count);

    for (const auto& player : msg.players) {
        writeToBuffer(it, player.first);
        writeToBuffer(it, player.second.x);
        writeToBuffer(it, player.second.y);

        // Prevent buffer overflow
        if (static_cast<size_t>(it - buffer.data()) >= BUFFER_SIZE) {
            break;
        }
    }

    return buffer;
}

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> RTypeProtocol::serialize(const EntityUpdateMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};
    char*                         it     = buffer.data();

    // Serialize the base message
    writeToBuffer(it, static_cast<const BaseMessage&>(msg));

    // Serialize the additional fields
    writeToBuffer(it, msg.x);
    writeToBuffer(it, msg.y);
    writeToBuffer(it, msg.angle);
    writeToBuffer(it, msg.step);
    writeToBuffer(it, msg.timestamp);

    return buffer;
}

template <std::size_t BUFFER_SIZE>
RTypeProtocol::DestroyEntityMessage RTypeProtocol::deserializeDestroyEntity(
    const std::array<char, BUFFER_SIZE>& buffer) {
    DestroyEntityMessage msg;
    const char*          it = buffer.data();

    // Deserialize the base message
    readFromBuffer(it, static_cast<BaseMessage&>(msg));

    // Deserialize the additional fields
    int entity_count = 0;
    readFromBuffer(it, entity_count);

    for (int i = 0; i < entity_count; ++i) {
        long entity_id;
        readFromBuffer(it, entity_id);
        msg.entity_ids.push_back(entity_id);
    }

    return msg;
}

template <std::size_t BUFFER_SIZE>
RTypeProtocol::PlayerDirectionMessage RTypeProtocol::deserializePlayerDirection(
    const std::array<char, BUFFER_SIZE>& buffer) {
    PlayerDirectionMessage msg;
    const char*            it = buffer.data();

    // Deserialize the base message
    readFromBuffer(it, static_cast<BaseMessage&>(msg));

    // Deserialize the additional fields
    readFromBuffer(it, msg.direction);
    readFromBuffer(it, msg.timestamp);

    return msg;
}

// Helper function to deserialize a PlayerMoveMessage
template <std::size_t BUFFER_SIZE>
RTypeProtocol::PlayerMoveMessage RTypeProtocol::deserializePlayerMove(
    const std::array<char, BUFFER_SIZE>& buffer) {
    PlayerMoveMessage msg;
    const char*       it = buffer.data();

    // Deserialize the base message
    readFromBuffer(it, static_cast<BaseMessage&>(msg));

    // Deserialize the additional fields
    readFromBuffer(it, msg.x);
    readFromBuffer(it, msg.y);
    readFromBuffer(it, msg.step);
    readFromBuffer(it, msg.timestamp);

    return msg;
}

// Deserialize an EventMessage
template <std::size_t BUFFER_SIZE>
RTypeProtocol::NewEntityMessage RTypeProtocol::deserializeNewEntityMessage(
    const std::array<char, BUFFER_SIZE>& buffer) {
    NewEntityMessage msg;
    const char*      it = buffer.data();

    // Deserialize the base message
    readFromBuffer(it, static_cast<BaseMessage&>(msg));

    // Deserialize the entity type
    readFromBuffer(it, msg.entity_type);

    // Deserialize each component by reading [componentType, dataLength, rawData]
    while (it + sizeof(int) + sizeof(int) <= buffer.data() + BUFFER_SIZE) {
        ComponentList compType;
        int           dataLength = 0;

        // Read the component type and the length of the data
        readFromBuffer(it, compType);
        readFromBuffer(it, dataLength);

        // Prevent overflows or invalid lengths
        if (dataLength <= 0 || it + dataLength > buffer.data() + BUFFER_SIZE) {
            break;
        }

        // Copy the raw bytes
        std::vector<char> componentData(it, it + dataLength);
        it += dataLength;

        msg.components.push_back({compType, componentData});
    }

    return msg;
}

template <std::size_t BUFFER_SIZE>
RTypeProtocol::SynchronizeMessage RTypeProtocol::deserializeSynchronize(
    const std::array<char, BUFFER_SIZE>& buffer) {
    SynchronizeMessage msg = {};
    const char*        it  = buffer.data();

    // Deserialize base message fields
    readFromBuffer(it, msg.message_type);
    readFromBuffer(it, msg.uuid);

    // Deserialize specific fields
    readFromBuffer(it, msg.timestamp);
    readFromBuffer(it, msg.x);
    readFromBuffer(it, msg.y);

    // Deserialize player UUID and position list
    int player_count = 0;
    readFromBuffer(it, player_count);

    for (int i = 0; i < player_count; ++i) {
        // Prevent overflow
        if (static_cast<size_t>(it - buffer.data()) + sizeof(long) + 2 * sizeof(float) >
            BUFFER_SIZE)
            break;

        long         uuid;
        sf::Vector2f position;
        readFromBuffer(it, uuid);
        readFromBuffer(it, position.x);
        readFromBuffer(it, position.y);

        msg.players.push_back({uuid, position});
    }

    return msg;
}

// Serialize the BaseMessage itself (simplified for serialization of standard BaseMessage)
template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> RTypeProtocol::serialize(const BaseMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};
    std::memcpy(buffer.data(), &msg, sizeof(msg));
    return buffer;
}

// Deserialize the BaseMessage
template <std::size_t BUFFER_SIZE>
RTypeProtocol::BaseMessage RTypeProtocol::deserialize(const std::array<char, BUFFER_SIZE>& buffer) {
    return deserializeBaseMessage<BUFFER_SIZE, BaseMessage>(buffer);
}

template <std::size_t BUFFER_SIZE>
RTypeProtocol::LevelSignalMessage RTypeProtocol::deserializeLevelSignal(
    const std::array<char, BUFFER_SIZE>& buffer) {
    LevelSignalMessage msg;
    std::size_t        offset = 0;

    // Deserialize base message
    std::memcpy(&msg, buffer.data() + offset, sizeof(BaseMessage));
    offset += sizeof(BaseMessage);

    // Deserialize startLevel
    std::memcpy(&msg.startLevel, buffer.data() + offset, sizeof(msg.startLevel));
    offset += sizeof(msg.startLevel);

    return msg;
}

template <std::size_t BUFFER_SIZE>
RTypeProtocol::EntityUpdateMessage RTypeProtocol::deserializeEntityUpdate(
    const std::array<char, BUFFER_SIZE>& buffer) {
    EntityUpdateMessage msg;
    const char*         it = buffer.data();

    // Deserialize the base message
    readFromBuffer(it, static_cast<BaseMessage&>(msg));

    // Deserialize the additional fields
    readFromBuffer(it, msg.x);
    readFromBuffer(it, msg.y);
    readFromBuffer(it, msg.angle);
    readFromBuffer(it, msg.step);
    readFromBuffer(it, msg.timestamp);

    return msg;
}

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> RTypeProtocol::serialize(const OneIntMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};
    char*                         it     = buffer.data();

    // Serialize the base message
    writeToBuffer(it, static_cast<const BaseMessage&>(msg));

    // Serialize the integer value
    writeToBuffer(it, msg.value);

    return buffer;
}

template <std::size_t BUFFER_SIZE>
RTypeProtocol::OneIntMessage RTypeProtocol::deserializeOneIntMessage(
    const std::array<char, BUFFER_SIZE>& buffer) {
    OneIntMessage msg;
    const char*   it = buffer.data();

    // Deserialize the base message
    readFromBuffer(it, static_cast<BaseMessage&>(msg));

    // Deserialize the integer value
    readFromBuffer(it, msg.value);

    return msg;
}

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> RTypeProtocol::serialize(const PlayerUpdateDataMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};
    char*                         it     = buffer.data();

    // Serialize the base message
    writeToBuffer(it, static_cast<const BaseMessage&>(msg));

    // Serialize the integer values
    writeToBuffer(it, msg.score);
    writeToBuffer(it, msg.health);

    return buffer;
}

template <std::size_t BUFFER_SIZE>
RTypeProtocol::PlayerUpdateDataMessage RTypeProtocol::deserializePlayerUpdateDataMessage(
    const std::array<char, BUFFER_SIZE>& buffer) {
    PlayerUpdateDataMessage msg;
    const char*             it = buffer.data();

    // Deserialize the base message
    readFromBuffer(it, static_cast<BaseMessage&>(msg));

    // Deserialize the integer values
    readFromBuffer(it, msg.score);
    readFromBuffer(it, msg.health);

    return msg;
}

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> RTypeProtocol::serialize(const ChangingSceneMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer;
    char*                         dest = buffer.data();

    // Serialize the base message
    writeToBuffer(dest, static_cast<const BaseMessage&>(msg));

    // Serialize the scene_id
    writeToBuffer(dest, static_cast<int>(msg.scene_id));

    return buffer;
}

template <std::size_t BUFFER_SIZE>
RTypeProtocol::ChangingSceneMessage RTypeProtocol::deserializeChangingSceneMessage(
    const std::array<char, BUFFER_SIZE>& buffer) {
    ChangingSceneMessage msg;
    const char*          src = buffer.data();

    // Deserialize the base message
    readFromBuffer(src, static_cast<BaseMessage&>(msg));

    // Deserialize the scene_id
    int scene_id;
    readFromBuffer(src, scene_id);
    msg.scene_id = static_cast<SceneType>(scene_id);

    return msg;
}

#endif  // RTYPESERVERPROTOCOL_TPP_

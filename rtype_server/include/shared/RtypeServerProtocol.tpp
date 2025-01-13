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
std::array<char, BUFFER_SIZE> RTypeProtocol::serialize(const RTypeProtocol::MapMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};
    std::size_t                   offset = 0;

    // Serialize base message
    std::memcpy(buffer.data() + offset, &msg, sizeof(BaseMessage));
    offset += sizeof(BaseMessage);

    // Serialize other fields
    std::memcpy(buffer.data() + offset, &msg.scrollingSpeed, sizeof(msg.scrollingSpeed));
    offset += sizeof(msg.scrollingSpeed);
    std::memcpy(buffer.data() + offset, &msg.x_level_position, sizeof(msg.x_level_position));
    offset += sizeof(msg.x_level_position);
    std::memcpy(buffer.data() + offset, &msg.isLoaded, sizeof(msg.isLoaded));
    offset += sizeof(msg.isLoaded);
    std::memcpy(buffer.data() + offset, &msg.isLevelRunning, sizeof(msg.isLevelRunning));
    offset += sizeof(msg.isLevelRunning);
    std::memcpy(buffer.data() + offset, &msg.server_tick, sizeof(msg.server_tick));
    offset += sizeof(msg.server_tick);

    // Serialize level_music vector size
    size_t level_music_size = msg.level_music.size();
    std::memcpy(buffer.data() + offset, &level_music_size, sizeof(level_music_size));
    offset += sizeof(level_music_size);

    // Serialize level_music vector data
    std::memcpy(buffer.data() + offset, msg.level_music.data(), level_music_size);
    offset += level_music_size;

    // Serialize backgrounds vector size
    size_t backgrounds_size = msg.backgrounds.size();
    std::memcpy(buffer.data() + offset, &backgrounds_size, sizeof(backgrounds_size));
    offset += sizeof(backgrounds_size);

    // Serialize each background
    for (const auto& background : msg.backgrounds) {
        // Serialize background data size
        size_t data_size = background.data.size();
        std::memcpy(buffer.data() + offset, &data_size, sizeof(data_size));
        offset += sizeof(data_size);

        // Serialize background data
        std::memcpy(buffer.data() + offset, background.data.data(), data_size);
        offset += data_size;

        // Serialize background position
        std::memcpy(buffer.data() + offset, &background.speed, sizeof(background.speed));
        offset += sizeof(background.speed);
    }

    return buffer;
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

    // Serialize the event type
    writeToBuffer(it, static_cast<int>(msg.entity_type));

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

    // Deserialize the event type
    int entityTypeInt;
    readFromBuffer(it, entityTypeInt);
    msg.entity_type = static_cast<EntityType>(entityTypeInt);

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
RTypeProtocol::MapMessage RTypeProtocol::deserializeMapMessage(
    const std::array<char, BUFFER_SIZE>& buffer) {
    MapMessage  msg;
    std::size_t offset = 0;

    // Deserialize base message
    std::memcpy(&msg, buffer.data() + offset, sizeof(BaseMessage));
    offset += sizeof(BaseMessage);

    // Deserialize other fields
    std::memcpy(&msg.scrollingSpeed, buffer.data() + offset, sizeof(msg.scrollingSpeed));
    offset += sizeof(msg.scrollingSpeed);
    std::memcpy(&msg.x_level_position, buffer.data() + offset, sizeof(msg.x_level_position));
    offset += sizeof(msg.x_level_position);
    std::memcpy(&msg.isLoaded, buffer.data() + offset, sizeof(msg.isLoaded));
    offset += sizeof(msg.isLoaded);
    std::memcpy(&msg.isLevelRunning, buffer.data() + offset, sizeof(msg.isLevelRunning));
    offset += sizeof(msg.isLevelRunning);
    std::memcpy(&msg.server_tick, buffer.data() + offset, sizeof(msg.server_tick));
    offset += sizeof(msg.server_tick);

    // Deserialize level_music vector size
    size_t level_music_size;
    std::memcpy(&level_music_size, buffer.data() + offset, sizeof(level_music_size));
    offset += sizeof(level_music_size);

    // Deserialize level_music vector data
    msg.level_music.resize(level_music_size);
    std::memcpy(msg.level_music.data(), buffer.data() + offset, level_music_size);
    offset += level_music_size;

    // Deserialize backgrounds vector size
    size_t backgrounds_size;
    std::memcpy(&backgrounds_size, buffer.data() + offset, sizeof(backgrounds_size));
    offset += sizeof(backgrounds_size);

    // Deserialize each background
    for (size_t i = 0; i < backgrounds_size; ++i) {
        BackgroundData background;

        // Deserialize background data size
        size_t data_size;
        std::memcpy(&data_size, buffer.data() + offset, sizeof(data_size));
        offset += sizeof(data_size);

        // Deserialize background data
        background.data.resize(data_size);
        std::memcpy(background.data.data(), buffer.data() + offset, data_size);
        offset += data_size;

        // Deserialize background position
        std::memcpy(&background.speed, buffer.data() + offset, sizeof(background.speed));
        offset += sizeof(background.speed);

        msg.backgrounds.push_back(background);
    }

    return msg;
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

#endif  // RTYPESERVERPROTOCOL_TPP_

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

// Serialize the BaseMessage, which is common across all message types
// template <std::size_t BUFFER_SIZE, typename T>
// std::array<char, BUFFER_SIZE> serializeBaseMessage(const T& msg) {
//     std::array<char, BUFFER_SIZE> buffer = {};
//     std::memcpy(buffer.data(), &msg, sizeof(msg));
//     return buffer;
// }
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

    // Serialize the base message
    std::array<char, BUFFER_SIZE> baseBuffer = serializeBaseMessage<BUFFER_SIZE>(msg);
    std::memcpy(buffer.data(), baseBuffer.data(), sizeof(BaseMessage));

    // Serialize the additional fields specific to PlayerMoveMessage
    std::memcpy(buffer.data() + sizeof(BaseMessage), &msg.x, sizeof(msg.x));
    std::memcpy(buffer.data() + sizeof(BaseMessage) + sizeof(msg.x), &msg.y, sizeof(msg.y));
    std::memcpy(buffer.data() + sizeof(BaseMessage) + sizeof(msg.x) + sizeof(msg.y), &msg.step,
                sizeof(msg.step));
    std::memcpy(
        buffer.data() + sizeof(BaseMessage) + sizeof(msg.x) + sizeof(msg.y) + sizeof(msg.step),
        &msg.timestamp, sizeof(msg.timestamp));

    return buffer;
}

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> RTypeProtocol::serialize(const PlayerDirectionMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};

    // Serialize the base message
    std::array<char, BUFFER_SIZE> baseBuffer = serializeBaseMessage<BUFFER_SIZE>(msg);
    std::memcpy(buffer.data(), baseBuffer.data(), sizeof(BaseMessage));

    // Serialize the additional fields specific to PlayerDirectionMessage
    std::memcpy(buffer.data() + sizeof(BaseMessage), &msg.direction, sizeof(msg.direction));
    std::memcpy(buffer.data() + sizeof(BaseMessage) + sizeof(msg.direction), &msg.timestamp,
                sizeof(msg.timestamp));

    return buffer;
}

template <std::size_t BUFFER_SIZE>
RTypeProtocol::PlayerDirectionMessage RTypeProtocol::deserializePlayerDirection(
    const std::array<char, BUFFER_SIZE>& buffer) {
    PlayerDirectionMessage msg;

    // Deserialize the base message first
    msg = deserializeBaseMessage<BUFFER_SIZE, PlayerDirectionMessage>(buffer);

    // Deserialize the specific fields for PlayerDirectionMessage
    std::memcpy(&msg.direction, buffer.data() + sizeof(BaseMessage), sizeof(msg.direction));
    std::memcpy(&msg.timestamp, buffer.data() + sizeof(BaseMessage) + sizeof(msg.direction),
                sizeof(msg.timestamp));

    return msg;
}

// Helper function to deserialize a PlayerMoveMessage
template <std::size_t BUFFER_SIZE>
RTypeProtocol::PlayerMoveMessage RTypeProtocol::deserializePlayerMove(
    const std::array<char, BUFFER_SIZE>& buffer) {
    PlayerMoveMessage msg;

    // Deserialize the base message first
    msg = deserializeBaseMessage<BUFFER_SIZE, PlayerMoveMessage>(buffer);

    // Deserialize the specific fields for PlayerMoveMessage
    std::memcpy(&msg.x, buffer.data() + sizeof(BaseMessage), sizeof(msg.x));
    std::memcpy(&msg.y, buffer.data() + sizeof(BaseMessage) + sizeof(msg.x), sizeof(msg.y));
    std::memcpy(&msg.step, buffer.data() + sizeof(BaseMessage) + sizeof(msg.x) + sizeof(msg.y),
                sizeof(msg.step));
    std::memcpy(
        &msg.timestamp,
        buffer.data() + sizeof(BaseMessage) + sizeof(msg.x) + sizeof(msg.y) + sizeof(msg.step),
        sizeof(msg.timestamp));

    return msg;
}

// Serialize an EventMessage
template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> RTypeProtocol::serialize(const EventMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};

    // Serialize the base message
    std::array<char, BUFFER_SIZE> baseBuffer = serializeBaseMessage<BUFFER_SIZE>(msg);
    std::memcpy(buffer.data(), baseBuffer.data(), sizeof(BaseMessage));

    // Serialize the components
    size_t offset = sizeof(BaseMessage);
    for (const auto& component : msg.components) {
        buffer[offset++] = static_cast<char>(component.first);  // Store the component type
        std::memcpy(&buffer[offset], component.second.data(), component.second.size());
        offset += component.second.size();
    }

    return buffer;
}

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> RTypeProtocol::serialize(const SynchronizeMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};
    std::size_t                   offset = 0;

    // Serialize base message fields
    std::memcpy(buffer.data() + offset, &msg.message_type, sizeof(msg.message_type));
    offset += sizeof(msg.message_type);
    std::memcpy(buffer.data() + offset, &msg.uuid, sizeof(msg.uuid));
    offset += sizeof(msg.uuid);

    // Serialize specific fields
    std::memcpy(buffer.data() + offset, &msg.timestamp, sizeof(msg.timestamp));
    offset += sizeof(msg.timestamp);
    std::memcpy(buffer.data() + offset, &msg.x, sizeof(msg.x));
    offset += sizeof(msg.x);
    std::memcpy(buffer.data() + offset, &msg.y, sizeof(msg.y));
    offset += sizeof(msg.y);

    // Serialize player UUID and position list
    int player_count = static_cast<int>(msg.players.size());
    std::memcpy(buffer.data() + offset, &player_count, sizeof(player_count));
    offset += sizeof(player_count);
    for (const auto& player : msg.players) {
        std::memcpy(buffer.data() + offset, &player.first, sizeof(player.first));
        offset += sizeof(player.first);
        std::memcpy(buffer.data() + offset, &player.second.x, sizeof(player.second.x));
        offset += sizeof(player.second.x);
        std::memcpy(buffer.data() + offset, &player.second.y, sizeof(player.second.y));
        offset += sizeof(player.second.y);
        if (offset >= BUFFER_SIZE) break;  // Prevent buffer overflow
    }

    return buffer;
}

// Deserialize an EventMessage
template <std::size_t BUFFER_SIZE>
RTypeProtocol::EventMessage RTypeProtocol::deserializeEventMessage(
    const std::array<char, BUFFER_SIZE>& buffer) {
    EventMessage msg;

    // Deserialize the base message
    msg = deserializeBaseMessage<BUFFER_SIZE, EventMessage>(buffer);

    // Deserialize the components
    size_t offset = sizeof(BaseMessage);
    while (offset < BUFFER_SIZE && buffer[offset] != '\0') {
        int componentType = static_cast<int>(buffer[offset]);
        offset++;  // Move past the component type byte

        // Find the component's payload size by checking until the end
        std::vector<char> componentData(buffer.begin() + offset, buffer.end());
        msg.components.push_back({componentType, componentData});

        offset += componentData.size();
    }

    return msg;
}

template <std::size_t BUFFER_SIZE>
RTypeProtocol::SynchronizeMessage RTypeProtocol::deserializeSynchronize(
    const std::array<char, BUFFER_SIZE>& buffer) {
    SynchronizeMessage msg    = {};
    std::size_t        offset = 0;

    // Deserialize base message fields
    std::memcpy(&msg.message_type, buffer.data() + offset, sizeof(msg.message_type));
    offset += sizeof(msg.message_type);
    std::memcpy(&msg.uuid, buffer.data() + offset, sizeof(msg.uuid));
    offset += sizeof(msg.uuid);

    // Deserialize specific fields
    std::memcpy(&msg.timestamp, buffer.data() + offset, sizeof(msg.timestamp));
    offset += sizeof(msg.timestamp);
    std::memcpy(&msg.x, buffer.data() + offset, sizeof(msg.x));
    offset += sizeof(msg.x);
    std::memcpy(&msg.y, buffer.data() + offset, sizeof(msg.y));
    offset += sizeof(msg.y);

    // Deserialize player UUID and position list
    int player_count = 0;
    std::memcpy(&player_count, buffer.data() + offset, sizeof(player_count));
    offset += sizeof(player_count);
    for (int i = 0; i < player_count; ++i) {
        if (offset + sizeof(long) + 2 * sizeof(float) > BUFFER_SIZE) break;  // Prevent overflow
        long         uuid;
        sf::Vector2f position;
        std::memcpy(&uuid, buffer.data() + offset, sizeof(uuid));
        offset += sizeof(uuid);
        std::memcpy(&position.x, buffer.data() + offset, sizeof(position.x));
        offset += sizeof(position.x);
        std::memcpy(&position.y, buffer.data() + offset, sizeof(position.y));
        offset += sizeof(position.y);
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

#endif  // RTYPESERVERPROTOCOL_TPP_

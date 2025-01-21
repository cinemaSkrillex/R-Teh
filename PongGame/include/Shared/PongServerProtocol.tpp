/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongServerProtocol.tpp
*/

#ifndef PONGSERVERPROTOCOL_TPP_
#define PONGSERVERPROTOCOL_TPP_

#include <cstring>
#include <iostream>

#include "PongServerProtocol.hpp"

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
std::array<char, BUFFER_SIZE> PongProtocol::serialize(const PlayerMoveMessage& msg) {
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
std::array<char, BUFFER_SIZE> PongProtocol::serialize(const PlayerDirectionMessage& msg) {
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
std::array<char, BUFFER_SIZE> PongProtocol::serialize(const NewEntityMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};
    char*                         it     = buffer.data();

    // Serialize the base message
    writeToBuffer(it, static_cast<const BaseMessage&>(msg));

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
std::array<char, BUFFER_SIZE> PongProtocol::serialize(const EntityUpdateMessage& msg) {
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
PongProtocol::PlayerDirectionMessage PongProtocol::deserializePlayerDirection(
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
PongProtocol::PlayerMoveMessage PongProtocol::deserializePlayerMove(
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

template <std::size_t BUFFER_SIZE>
PongProtocol::NewEntityMessage PongProtocol::deserializeNewEntity(
    const std::array<char, BUFFER_SIZE>& buffer) {
    NewEntityMessage msg;
    const char*      it = buffer.data();

    // Deserialize the base message
    readFromBuffer(it, static_cast<BaseMessage&>(msg));

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
std::array<char, BUFFER_SIZE> PongProtocol::serialize(const BaseMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};
    std::memcpy(buffer.data(), &msg, sizeof(msg));
    return buffer;
}

// Deserialize the BaseMessage
template <std::size_t BUFFER_SIZE>
PongProtocol::BaseMessage PongProtocol::deserialize(const std::array<char, BUFFER_SIZE>& buffer) {
    return deserializeBaseMessage<BUFFER_SIZE, BaseMessage>(buffer);
}

template <std::size_t BUFFER_SIZE>
PongProtocol::EntityUpdateMessage PongProtocol::deserializeEntityUpdate(
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
std::array<char, BUFFER_SIZE> PongProtocol::serialize(const PlayerUpdateDataMessage& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};
    char*                         it     = buffer.data();

    // Serialize the base message
    writeToBuffer(it, static_cast<const BaseMessage&>(msg));

    // Serialize the integer values
    writeToBuffer(it, msg.score);

    return buffer;
}

template <std::size_t BUFFER_SIZE>
PongProtocol::PlayerUpdateDataMessage PongProtocol::deserializePlayerUpdateData(
    const std::array<char, BUFFER_SIZE>& buffer) {
    PlayerUpdateDataMessage msg;
    const char*             it = buffer.data();

    // Deserialize the base message
    readFromBuffer(it, static_cast<BaseMessage&>(msg));

    // Deserialize the integer values
    readFromBuffer(it, msg.score);

    return msg;
}

#endif  // PONGSERVERPROTOCOL_TPP_
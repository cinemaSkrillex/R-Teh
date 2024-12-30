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

// Serialize the BaseMessage, which is common across all message types
template <std::size_t BUFFER_SIZE, typename T>
std::array<char, BUFFER_SIZE> serializeBaseMessage(const T& msg) {
    std::array<char, BUFFER_SIZE> buffer = {};
    std::memcpy(buffer.data(), &msg, sizeof(msg));
    return buffer;
}

// Deserialize a BaseMessage from the buffer
template <std::size_t BUFFER_SIZE, typename T>
T deserializeBaseMessage(const std::array<char, BUFFER_SIZE>& buffer) {
    T msg;
    std::memcpy(&msg, buffer.data(), sizeof(msg));
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
    std::memcpy(buffer.data() + sizeof(BaseMessage) + sizeof(msg.x) + sizeof(msg.y), &msg.timestamp,
                sizeof(msg.timestamp));

    return buffer;
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
    std::memcpy(&msg.timestamp, buffer.data() + sizeof(BaseMessage) + sizeof(msg.x) + sizeof(msg.y),
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

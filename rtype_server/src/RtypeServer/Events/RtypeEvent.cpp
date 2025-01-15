/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeEvent
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

void RtypeServer::runEvent(const std::array<char, 800>&   buffer,
                           const asio::ip::udp::endpoint& client, ServerPlayer& player) {
    auto baseMessage = RTypeProtocol::deserialize<800>(buffer);
    auto it          = eventHandlers.find(baseMessage.message_type);
    if (it != eventHandlers.end()) {
        it->second->execute(buffer, client, player, this);
    } else {
        std::cerr << "Unknown event type: " << baseMessage.message_type << std::endl;
    }
}
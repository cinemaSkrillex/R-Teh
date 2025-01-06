/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeEvent
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

void RtypeServer::runEvent(const std::array<char, 800>&   buffer,
                           const asio::ip::udp::endpoint& client, Player& player) {
    RTypeProtocol::BaseMessage baseMessage = RTypeProtocol::deserialize<800>(buffer);

    if (baseMessage.message_type == RTypeProtocol::MessageType::SHOOT_EVENT) {
        shootEvent(buffer, client, player);
        return;
    }
    std::cout << "Unknown message type: " << baseMessage.message_type << std::endl;
}
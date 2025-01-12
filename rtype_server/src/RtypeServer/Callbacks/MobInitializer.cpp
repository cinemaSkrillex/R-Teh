/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** MobInitializer
*/

#include "MobInitializer.hpp"

void MobInitializer::initializeMobs(const asio::ip::udp::endpoint& sender) {
    for (const auto& mob : _gameInstance->getSimpleMobs()) {
        if (!mob) continue;

        RTypeProtocol::NewEntityMessage eventMessage;
        eventMessage.message_type = RTypeProtocol::MessageType::NEW_ENTITY;
        eventMessage.uuid         = *mob;

        // Serialize and add components
        auto registry = _gameInstance->getRegistry();
        // register the components

        std::array<char, 800> serializedMessage = RTypeProtocol::serialize<800>(eventMessage);
        _UdpServer->send_reliable_packet(serializedMessage, sender);
    }
}
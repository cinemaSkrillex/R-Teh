/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RtypeServerCallbackUtils
*/

#include "RtypeServer/RtypeServer.hpp"

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> createNewClientMessage(long uuid, float x, float y, long timestamp) {
    // Populate the NewClientMessage structure
    RTypeProtocol::PlayerMoveMessage newClientMessage = {};
    newClientMessage.message_type                     = RTypeProtocol::NEW_CLIENT;  // Message type
    newClientMessage.uuid                             = uuid;                       // Player UUID
    newClientMessage.x                                = x;          // Player X position
    newClientMessage.y                                = y;          // Player Y position
    newClientMessage.timestamp                        = timestamp;  // Synchronization timestamp

    // Serialize the message
    return RTypeProtocol::serialize<BUFFER_SIZE>(newClientMessage);
}

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> createSynchronizeMessage(
    long uuid, long timestamp, float x, float y,
    const std::vector<std::pair<long, sf::Vector2f>>& activePlayerUUIDs) {
    // Populate the SynchronizeMessage structure
    RTypeProtocol::SynchronizeMessage syncMessage;
    syncMessage.message_type = RTypeProtocol::SYNCHRONISE;  // Message type
    syncMessage.uuid         = uuid;                        // Player UUID
    syncMessage.timestamp    = timestamp;                   // Synchronization timestamp
    syncMessage.x            = x;                           // Player X position
    syncMessage.y            = y;                           // Player Y position
    syncMessage.players      = activePlayerUUIDs;           // List of active player UUIDs

    // Serialize the message
    return RTypeProtocol::serialize<BUFFER_SIZE>(syncMessage);
}

void RtypeServer::sendNewClientMessages(const asio::ip::udp::endpoint& sender, long playerEntity,
                                        float x, float y, long timestamp) {
    for (const auto& client : _server->getClients()) {
        if (client != sender) {
            if (!_players[client].getUUID()) {
                std::cout << "Player UUID is null" << std::endl;
                continue;
            }
            std::array<char, 800> serializedMessage =
                createNewClientMessage<800>(playerEntity, x, y, timestamp);
            _server->send_reliable_packet(serializedMessage, client);
        }
    }
}

void RtypeServer::sendSynchronizeMessage(const asio::ip::udp::endpoint& sender, long playerEntity,
                                         const sf::Vector2f& player_start_position,
                                         long                timestamp) {
    std::vector<std::pair<long, sf::Vector2f>> activePlayerUUIDs;
    for (const auto& player_pair : _players) {
        const auto& player = player_pair.second;
        if (!player.getUUID()) {
            std::cout << "Player UUID is null" << std::endl;
            continue;
        }
        std::cout << "Player UUID: " << player.getUUID() << std::endl;
        activePlayerUUIDs.push_back({player.getUUID(), player.getPosition()});
    }
    std::array<char, 800> synchronizeMessage =
        createSynchronizeMessage<800>(playerEntity, timestamp, player_start_position.x,
                                      player_start_position.y, activePlayerUUIDs);

    _server->send_reliable_packet(synchronizeMessage, sender);
}
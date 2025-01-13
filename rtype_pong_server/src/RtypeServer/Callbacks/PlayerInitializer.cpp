/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** PlayerInitializer
*/

#include "PlayerInitializer.hpp"

Player PlayerInitializer::initializePlayer(const asio::ip::udp::endpoint& sender) {
    auto gameInstance = _server->getGameInstance();
    long elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::steady_clock::now() - _server->getStartTime())
                            .count();
    sf::Vector2f player_start_position =
        _server->getServerConfig().getConfigItem<sf::Vector2f>("PLAYER_START_POSITION");
    auto   playerEntity = gameInstance->addAndGetPlayer(player_start_position);
    Player player = Player(*playerEntity, elapsed_time, playerEntity, gameInstance->getRegistry());

    sendNewClientMessages(sender, *playerEntity, player_start_position, elapsed_time);
    sendSynchronizeMessage(sender, *playerEntity, player_start_position, elapsed_time);
    return player;
}

template <std::size_t BUFFER_SIZE>
std::array<char, BUFFER_SIZE> createNewClientMessage(long uuid, sf::Vector2f player_start_pos,
                                                     long timestamp) {
    // Populate the NewClientMessage structure
    RTypeProtocol::PlayerMoveMessage newClientMessage = {};
    newClientMessage.message_type                     = RTypeProtocol::NEW_CLIENT;  // Message type
    newClientMessage.uuid                             = uuid;                       // Player UUID
    newClientMessage.x                                = player_start_pos.x;  // Player X position
    newClientMessage.y                                = player_start_pos.y;  // Player Y position
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
    syncMessage.message_type = RTypeProtocol::SYNCHRONIZE;  // Message type
    syncMessage.uuid         = uuid;                        // Player UUID
    syncMessage.timestamp    = timestamp;                   // Synchronization timestamp
    syncMessage.x            = x;                           // Player X position
    syncMessage.y            = y;                           // Player Y position
    syncMessage.entities      = activePlayerUUIDs;           // List of active player UUIDs

    // Serialize the message
    return RTypeProtocol::serialize<BUFFER_SIZE>(syncMessage);
}

void PlayerInitializer::sendNewClientMessages(const asio::ip::udp::endpoint& sender,
                                              long playerEntity, sf::Vector2f player_start_pos,
                                              long timestamp) {
    auto                  UdpServer     = _server->getServer();
    const auto&           serverPlayers = _server->getPlayers();
    std::array<char, 800> serializedMessage;

    for (const auto& client : UdpServer->getClients()) {
        if (client != sender) {
            const auto& currentPlayer = serverPlayers.at(client);
            if (!currentPlayer.getUUID()) {
                std::cout << "Player UUID is null" << std::endl;
                continue;
            }
            serializedMessage =
                createNewClientMessage<800>(playerEntity, player_start_pos, timestamp);
            UdpServer->send_reliable_packet(serializedMessage, client);
        }
    }
}

void PlayerInitializer::sendSynchronizeMessage(const asio::ip::udp::endpoint& sender,
                                               long                           playerEntity,
                                               const sf::Vector2f&            player_start_position,
                                               long                           timestamp) {
    const auto&                                serverPlayers = _server->getPlayers();
    auto                                       UdpServer     = _server->getServer();
    std::array<char, 800>                      synchronizeMessage;
    std::vector<std::pair<long, sf::Vector2f>> activePlayerUUIDs;
    activePlayerUUIDs.reserve(_server->getPlayers().size());

    for (const auto& player_pair : serverPlayers) {
        const auto& current_player = player_pair.second;
        if (!current_player.getUUID()) {
            std::cout << "Player UUID is null" << std::endl;
            continue;
        }
        std::cout << "Player UUID: " << current_player.getUUID() << std::endl;
        activePlayerUUIDs.emplace_back(current_player.getUUID(), current_player.getPosition());
    }
    synchronizeMessage =
        createSynchronizeMessage<800>(playerEntity, timestamp, player_start_position.x,
                                      player_start_position.y, activePlayerUUIDs);

    UdpServer->send_reliable_packet(synchronizeMessage, sender);
}

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
        const auto& current_player = player_pair.second;
        if (!current_player.getUUID()) {
            std::cout << "Player UUID is null" << std::endl;
            continue;
        }
        std::cout << "Player UUID: " << current_player.getUUID() << std::endl;
        activePlayerUUIDs.push_back({current_player.getUUID(), current_player.getPosition()});
    }
    std::array<char, 800> synchronizeMessage =
        createSynchronizeMessage<800>(playerEntity, timestamp, player_start_position.x,
                                      player_start_position.y, activePlayerUUIDs);

    _server->send_reliable_packet(synchronizeMessage, sender);
}

void RtypeServer::processBatchMessages(std::vector<std::array<char, 800>>& batchMessages,
                                       const std::string&                  entityType) {
    if (!batchMessages.empty()) {
        for (const auto& message : batchMessages) {
            broadcastAllReliable(message);
        }
        std::cout << "Processed final batch of " << batchMessages.size() << " " << entityType
                  << " entities." << std::endl;
        batchMessages.clear();
    }
}

void RtypeServer::processTile(const Map::Tile&                    tile,
                              std::vector<std::array<char, 800>>& batchMessages) {
    if (tile.element.empty() || tile.position.x < 0 || tile.position.y < 0 || tile.rotation < 0) {
        std::cerr << "Error: Invalid tile data. Skipping..." << std::endl;
        return;
    }

    if (tile.type == "BLOCK") {
        if (!_game_instance) {
            std::cerr << "Error: Game instance is null" << std::endl;
            return;
        }
        auto newBlock    = std::make_shared<rtype::Block>(_game_instance->getRegistryRef(),
                                                          tile.position, tile.element, tile.rotation);
        auto blockEntity = newBlock->getEntity();

        if (!blockEntity) {
            std::cerr << "Error: Block entity is null" << std::endl;
            return;
        }

        RTypeProtocol::NewEntityMessage newTileMessage;
        newTileMessage.message_type = RTypeProtocol::MessageType::NEW_ENTITY;
        newTileMessage.uuid         = *blockEntity;

        addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::POSITION,
                              tile.position);
        addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::ROTATION,
                              tile.rotation);
        addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::DRAWABLE, true);

        sf::FloatRect bounds = {0, 0, 16, 8};
        addCollisionComponentToMessage(newTileMessage, bounds, tile.element, false,
                                       RealEngine::CollisionType::SOLID);

        std::string       sprite = tile.element;
        std::vector<char> spriteData(sprite.begin(), sprite.end());
        addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::SPRITE, spriteData);

        std::array<char, 800> serializedMessage = RTypeProtocol::serialize<800>(newTileMessage);
        batchMessages.push_back(serializedMessage);
    }
}

void RtypeServer::processWave(const Map::Wave&                    wave,
                              std::vector<std::array<char, 800>>& batchMessages) {
    for (const auto& content : wave.contents) {
        // Handle wave content (e.g., spawn mobs)
        std::cout << "Processing wave content: " << content.mobName << " at position ("
                  << content.position.x << ", " << content.position.y << ")" << std::endl;

        // TODO: create a Wave class and use it to create the entity
        //  Create a new entity for the wave content
        //  auto newEntity = std::make_shared<rtype::Block>(_game_instance->getRegistryRef(),
        //                                                  content.position, content.mobName,
        //                                                  0.0f);
        //  auto entity    = newEntity->getEntity();

        // if (!entity) {
        //     std::cerr << "Error: Wave entity is null" << std::endl;
        //     continue;
        // }

        RTypeProtocol::NewEntityMessage newEntityMessage;
        // newEntityMessage.message_type = RTypeProtocol::MessageType::NEW_ENTITY;
        // newEntityMessage.uuid         = *entity;

        // addComponentToMessage(newEntityMessage, RTypeProtocol::ComponentList::POSITION,
        //                       content.position);
        // addComponentToMessage(newEntityMessage, RTypeProtocol::ComponentList::DRAWABLE, true);

        // std::array<char, 800> serializedMessage =
        // RTypeProtocol::serialize<800>(newEntityMessage);
        // batchMessages.push_back(serializedMessage);
    }
}
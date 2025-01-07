/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerCallbacks
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

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

void RtypeServer::init_callback_mobs(const asio::ip::udp::endpoint& sender) {
    for (const auto& mob : _game_instance->getSimpleMobs()) {
        if (!mob) continue;

        auto* position = _game_instance->getRegistry()->get_component<RealEngine::Position>(mob);
        auto* destructible =
            _game_instance->getRegistry()->get_component<RealEngine::AutoDestructible>(mob);
        auto* velocity = _game_instance->getRegistry()->get_component<RealEngine::Velocity>(mob);
        auto* rotation = _game_instance->getRegistry()->get_component<RealEngine::Rotation>(mob);

        if (!position || !destructible || !velocity) continue;

        RTypeProtocol::NewEntityMessage eventMessage;
        eventMessage.message_type = RTypeProtocol::MessageType::NEW_ENTITY;
        eventMessage.uuid         = *mob;

        // Serialize position and velocity component
        addComponentToMessage(eventMessage, RTypeProtocol::ComponentList::POSITION, *position);
        addComponentToMessage(eventMessage, RTypeProtocol::ComponentList::VELOCITY, *velocity);

        // Serialize rotation component
        if (rotation) {
            std::vector<char> rotationData(sizeof(RealEngine::Rotation));
            std::memcpy(rotationData.data(), rotation, sizeof(RealEngine::Rotation));
            eventMessage.components.push_back(
                {RTypeProtocol::ComponentList::ROTATION, rotationData});
        }

        // Serialize collision component
        sf::FloatRect             bounds      = {0, 0, 16, 8};
        std::string               id          = "mob";
        bool                      isColliding = false;
        RealEngine::CollisionType type        = RealEngine::CollisionType::OTHER;

        std::vector<char> collisionData(sizeof(bounds) + id.size() + 1 + sizeof(isColliding) +
                                        sizeof(type));
        char*             collisionPtr = collisionData.data();
        std::memcpy(collisionPtr, &bounds, sizeof(bounds));
        collisionPtr += sizeof(bounds);
        std::memcpy(collisionPtr, id.c_str(), id.size() + 1);
        collisionPtr += id.size() + 1;
        std::memcpy(collisionPtr, &isColliding, sizeof(isColliding));
        collisionPtr += sizeof(isColliding);
        std::memcpy(collisionPtr, &type, sizeof(type));
        eventMessage.components.push_back({RTypeProtocol::ComponentList::COLLISION, collisionData});

        // Serialize auto destructible component
        float autoDestructible = destructible->lifeTime;
        addComponentToMessage(eventMessage, RTypeProtocol::ComponentList::AUTO_DESTRUCTIBLE,
                              autoDestructible);
        // Serialize drawable component
        bool drawable = true;
        addComponentToMessage(eventMessage, RTypeProtocol::ComponentList::DRAWABLE, drawable);

        // Serialize sprite component
        std::string       sprite = "eye_bomber";
        std::vector<char> spriteData(sprite.begin(), sprite.end());
        addComponentToMessage(eventMessage, RTypeProtocol::ComponentList::SPRITE, spriteData);

        std::array<char, 800> serializedEventMessage = RTypeProtocol::serialize<800>(eventMessage);
        _server->send_reliable_packet(serializedEventMessage, sender);
    }
}

Player RtypeServer::init_callback_players(const asio::ip::udp::endpoint& sender) {
    sf::Vector2f player_start_position =
        _server_config.getConfigItem<sf::Vector2f>("PLAYER_START_POSITION");
    // create Player entity
    long elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::steady_clock::now() - _startTime)
                            .count();
    auto playerEntity = _game_instance->addAndGetPlayer(player_start_position);
    auto player = Player(*playerEntity, elapsed_time, playerEntity, _game_instance->getRegistry());
    // Notify all other clients about the new client
    for (const auto& client : _server->getClients()) {
        if (client != sender) {
            std::array<char, 800> serializedMessage = createNewClientMessage<800>(
                *playerEntity, player_start_position.x, player_start_position.y, elapsed_time);
            _server->send_reliable_packet(serializedMessage, client);
        }
    }
    // Create the uuid for each new client
    std::vector<std::pair<long, sf::Vector2f>> activePlayerUUIDs;
    for (const auto& player_pair : _players) {
        const auto& player = player_pair.second;
        activePlayerUUIDs.push_back({player.getUUID(), player.getPosition()});
    }
    std::array<char, 800> synchronizeMessage = createSynchronizeMessage<800>(
        *playerEntity,
        std::chrono::duration_cast<std::chrono::milliseconds>(_startTime.time_since_epoch())
            .count(),
        player_start_position.x, player_start_position.y, activePlayerUUIDs);

    _server->send_reliable_packet(synchronizeMessage, sender);
    return player;
}

void RtypeServer::init_callback_map(const asio::ip::udp::endpoint& sender) {
    std::vector<RTypeProtocol::Tile> tiles = _server_map->getTiles();
    for (const auto& tile : tiles) {
        std::cout << "Tile: " << tile.position.x << " " << tile.position.y << std::endl;
        std::cout << "Tile: " << tile.type << " " << tile.element << "rotation " << tile.rotation
                  << std::endl;
        RTypeProtocol::NewEntityMessage newTileMessage;
        newTileMessage.message_type = RTypeProtocol::MessageType::NEW_ENTITY;
        newTileMessage.uuid         = 1000;

        if (tile.type == "BLOCK") {
            rtype::Block newBlock(*_game_instance->getRegistry(), tile.position, tile.element,
                                  tile.rotation);

            std::cout << "Creating block" << std::endl;
            addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::POSITION,
                                  tile.position);
            addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::ROTATION,
                                  tile.rotation);
            addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::DRAWABLE, true);
            addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::AUTO_DESTRUCTIBLE,
                                  50.0f);

            sf::FloatRect             bounds      = {0, 0, 16, 8};
            std::string               id          = tile.element;
            bool                      isColliding = false;
            RealEngine::CollisionType type        = RealEngine::CollisionType::SOLID;

            std::vector<char> collisionData(sizeof(bounds) + id.size() + 1 + sizeof(isColliding) +
                                            sizeof(type));
            char*             collisionPtr = collisionData.data();
            std::memcpy(collisionPtr, &bounds, sizeof(bounds));
            collisionPtr += sizeof(bounds);
            std::memcpy(collisionPtr, id.c_str(), id.size() + 1);
            collisionPtr += id.size() + 1;
            std::memcpy(collisionPtr, &isColliding, sizeof(isColliding));
            collisionPtr += sizeof(isColliding);
            std::memcpy(collisionPtr, &type, sizeof(type));
            newTileMessage.components.push_back(
                {RTypeProtocol::ComponentList::COLLISION, collisionData});

            std::string       sprite = tile.element;
            std::vector<char> spriteData(sprite.begin(), sprite.end());
            addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::SPRITE, spriteData);

            std::array<char, 800> serializedMessage = RTypeProtocol::serialize<800>(newTileMessage);
            broadcastAllReliable(serializedMessage);
            std::cout << "Broadcasted block" << std::endl;
        }
    }
}

void RtypeServer::initCallbacks() {
    _server->setNewClientCallback([this](const asio::ip::udp::endpoint& sender) {
        Player player = init_callback_players(sender);

        // Send all the entities to the new client, so it can synchronize and move
        init_callback_mobs(sender);

        init_callback_map(sender);
        _players[sender] = player;
    });
}

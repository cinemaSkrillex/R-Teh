/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerCallbacks
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

void RtypeServer::init_callback_mobs(const asio::ip::udp::endpoint& sender) {
    for (const auto& mob : _game_instance->getSimpleMobs()) {
        if (!mob) continue;

        auto* position = _game_instance->getRegistry()->get_component<RealEngine::Position>(mob);
        auto* destructible =
            _game_instance->getRegistry()->get_component<RealEngine::AutoDestructible>(mob);
        auto* velocity  = _game_instance->getRegistry()->get_component<RealEngine::Velocity>(mob);
        auto* rotation  = _game_instance->getRegistry()->get_component<RealEngine::Rotation>(mob);
        auto* collision = _game_instance->getRegistry()->get_component<RealEngine::Collision>(mob);
        auto* drawable  = _game_instance->getRegistry()->get_component<RealEngine::Drawable>(mob);
        auto* container =
            _game_instance->getRegistry()->get_component<RealEngine::NetvarContainer>(mob);
        auto* sprite =
            _game_instance->getRegistry()->get_component<RealEngine::SpriteComponent>(mob);
        auto* spriteSheet =
            _game_instance->getRegistry()->get_component<RealEngine::SpriteSheet>(mob);

        if (!container) {
            std::cerr << "Error: mob does not have a NetvarContainer component" << std::endl;
            continue;
        }
        if (!position || !velocity) {
            std::cerr << "Error: mob does not have a Position or Velocity component" << std::endl;
            continue;
        }

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
        if (collision) {
            sf::FloatRect             bounds      = collision->bounds;
            std::string               id          = collision->id;
            bool                      isColliding = collision->isColliding;
            RealEngine::CollisionType type        = collision->type;
            addCollisionComponentToMessage(eventMessage, bounds, id, isColliding, type);
        }
        // Serialize auto destructible component
        if (destructible) {
            float autoDestructible = destructible->lifeTime;
            addComponentToMessage(eventMessage, RTypeProtocol::ComponentList::AUTO_DESTRUCTIBLE,
                                  autoDestructible);
        }

        // Serialize drawable component
        if (drawable) {
            bool drawable = true;
            addComponentToMessage(eventMessage, RTypeProtocol::ComponentList::DRAWABLE, drawable);
        }

        // Serialize sprite component

        // if (container->getNetvar("sprite_name")) {
        // if (sprite) {

        // auto netvar = container->getNetvar("dropChance");
        // if (netvar && netvar->value.type() == typeid(float)) {
        //     float dropChance = std::any_cast<float>(netvar->value);
        //     std::cout << "DropChance: " << dropChance << std::endl;
        //     // std::vector<char> spriteData(sprite_str.begin(), sprite_str.end());
        //     // addComponentToMessage(eventMessage, RTypeProtocol::ComponentList::SPRITE,
        //     // spriteData);
        // } else {
        //     std::cerr << "Error: dropChance is not of type float or is null" << std::endl;
        // }

        auto netvar = container->getNetvar("sprite_name");
        if (netvar && netvar->value.type() == typeid(std::string)) {
            std::string sprite_str = std::any_cast<std::string>(netvar->value);
            std::cout << "Sprite: " << sprite_str << std::endl;
            std::vector<char> spriteData(sprite_str.begin(), sprite_str.end());
            addComponentToMessage(eventMessage, RTypeProtocol::ComponentList::SPRITE, spriteData);
        } else {
            std::cerr << "Error: sprite_name is not of type std::string or is null" << std::endl;
        }

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
    sendNewClientMessages(sender, *playerEntity, player_start_position.x, player_start_position.y,
                          elapsed_time);
    // Create the uuid for each new client
    sendSynchronizeMessage(sender, *playerEntity, player_start_position, elapsed_time);
    // std::vector<std::pair<long, sf::Vector2f>> activePlayerUUIDs;
    // for (const auto& player_pair : _players) {
    //     const auto& player = player_pair.second;
    //     if (!player.getUUID()) {
    //         std::cout << "Player UUID is null" << std::endl;
    //         continue;
    //     }
    //     std::cout << "Player UUID: " << player.getUUID() << std::endl;
    //     activePlayerUUIDs.push_back({player.getUUID(), player.getPosition()});
    // }
    // std::array<char, 800> synchronizeMessage = createSynchronizeMessage<800>(
    //     *playerEntity,
    //     std::chrono::duration_cast<std::chrono::milliseconds>(_startTime.time_since_epoch())
    //         .count(),
    //     player_start_position.x, player_start_position.y, activePlayerUUIDs);

    // _server->send_reliable_packet(synchronizeMessage, sender);
    return player;
}

// void RtypeServer::init_callback_map(const asio::ip::udp::endpoint& sender) {
//     if (!_server_map) {
//         std::cerr << "Error: Server map is null" << std::endl;
//         return;
//     }
//     std::vector<RTypeProtocol::Tile> tiles      = _server_map->getTiles();
//     int                              uuid_count = 0;
//     for (const auto& tile : tiles) {
//         RTypeProtocol::NewEntityMessage newTileMessage;
//         newTileMessage.message_type = RTypeProtocol::MessageType::NEW_ENTITY;
//         if (tile.element.empty() || tile.position.x < 0 || tile.position.y < 0 ||
//             tile.rotation < 0) {
//             std::cerr << "Error: Tile position is null" << std::endl;
//             return;
//         }

//         if (tile.type == "BLOCK") {
//             if (!_game_instance) {
//                 std::cerr << "Error: Game instance is null" << std::endl;
//                 return;
//             }
//             rtype::Block newBlock(*_game_instance->getRegistry(), tile.position, tile.element,
//                                   tile.rotation);
//             auto         blockEntity = newBlock.getEntity();
//             if (!blockEntity) {
//                 std::cerr << "Error: Block entity is null" << std::endl;
//                 return;
//             }
//             newTileMessage.uuid = *blockEntity;

//             addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::POSITION,
//                                   tile.position);
//             addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::ROTATION,
//                                   tile.rotation);
//             addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::DRAWABLE, true);

//             sf::FloatRect             bounds      = {0, 0, 16, 8};
//             std::string               id          = tile.element;
//             bool                      isColliding = false;
//             RealEngine::CollisionType type        = RealEngine::CollisionType::SOLID;

//             addCollisionComponentToMessage(newTileMessage, bounds, id, isColliding, type);

//             std::string       sprite = tile.element;
//             std::vector<char> spriteData(sprite.begin(), sprite.end());
//             addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::SPRITE,
//             spriteData);

//             std::array<char, 800> serializedMessage =
//             RTypeProtocol::serialize<800>(newTileMessage);
//             broadcastAllReliable(serializedMessage);
//             std::cout << "Broadcasted block" << std::endl;
//         }
//     }
// }
void RtypeServer::init_callback_map(const asio::ip::udp::endpoint& sender) {
    auto serverMap = _server_map;  // Retain shared ownership

    if (!serverMap) {
        std::cerr << "Error: Server map is null" << std::endl;
        return;
    }

    const std::vector<RTypeProtocol::Tile>& tiles = serverMap->getTiles();

    // Batching setup
    constexpr size_t BATCH_SIZE     = 100;  // Adjust as necessary for optimal performance
    size_t           processedCount = 0;

    std::vector<std::array<char, 800>> batchMessages;
    batchMessages.reserve(BATCH_SIZE);

    for (const auto& tile : tiles) {
        if (tile.element.empty() || tile.position.x < 0 || tile.position.y < 0 ||
            tile.rotation < 0) {
            std::cerr << "Error: Invalid tile data. Skipping..." << std::endl;
            continue;
        }

        if (tile.type == "BLOCK") {
            if (!_game_instance) {
                std::cerr << "Error: Game instance is null" << std::endl;
                return;
            }

            // rtype::Block newBlock(_game_instance->getRegistryRef(), tile.position, tile.element,
            //                       tile.rotation);
            // auto         blockEntity = newBlock.getEntity();
            auto newBlock = std::make_shared<rtype::Block>(
                _game_instance->getRegistryRef(), tile.position, tile.element, tile.rotation);
            auto blockEntity = newBlock->getEntity();

            if (!blockEntity) {
                std::cerr << "Error: Block entity is null" << std::endl;
                continue;
            }

            RTypeProtocol::NewEntityMessage newTileMessage;
            newTileMessage.message_type = RTypeProtocol::MessageType::NEW_ENTITY;
            newTileMessage.uuid         = *blockEntity;

            addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::POSITION,
                                  tile.position);
            addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::ROTATION,
                                  tile.rotation);
            addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::DRAWABLE, true);

            // sf::FloatRect bounds = {0, 0, 16, 8};
            // addCollisionComponentToMessage(newTileMessage, bounds, tile.element,
            // false,
            //                                RealEngine::CollisionType::SOLID);

            std::string       sprite = tile.element;
            std::vector<char> spriteData(sprite.begin(), sprite.end());
            addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::SPRITE, spriteData);

            std::array<char, 800> serializedMessage = RTypeProtocol::serialize<800>(newTileMessage);
            batchMessages.push_back(serializedMessage);

            processedCount++;

            // Send the batch if it reaches the batch size
            if (batchMessages.size() == BATCH_SIZE) {
                for (const auto& message : batchMessages) {
                    broadcastAllReliable(message);
                }
                batchMessages.clear();
                std::cout << "Processed batch of " << BATCH_SIZE << " tiles." << std::endl;
            }
        }
    }

    // Send remaining messages in the last batch
    if (!batchMessages.empty()) {
        for (const auto& message : batchMessages) {
            broadcastAllReliable(message);
        }
        std::cout << "Processed final batch of " << batchMessages.size() << " tiles." << std::endl;
    }

    //     std::cout << "Total processed tiles: " << processedCount << std::endl;
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

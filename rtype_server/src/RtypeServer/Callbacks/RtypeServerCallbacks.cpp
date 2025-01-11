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
    return player;
}

void RtypeServer::init_callback_map(const asio::ip::udp::endpoint& sender) {
    auto GameMap = _game_instance->getMap();

    if (!GameMap) {
        std::cerr << "Error: Server map is null" << std::endl;
        return;
    }

    const std::vector<Map::Wave>&                     waves          = GameMap->getWaves();
    const std::vector<std::shared_ptr<rtype::Block>>& blocks         = GameMap->getBlockEntities();
    float                                             scrollingSpeed = GameMap->getScrollingSpeed();
    float                                             xLevelPosition = GameMap->getXLevelPosition();

    RTypeProtocol::MapMessage mapMessage;
    mapMessage.message_type     = RTypeProtocol::MessageType::MAP_INFO;
    mapMessage.uuid             = 0;
    mapMessage.scrollingSpeed   = scrollingSpeed;
    mapMessage.x_level_position = xLevelPosition;

    std::array<char, 800> serializedMapMessage = RTypeProtocol::serialize<800>(mapMessage);
    broadcastAllReliable(serializedMapMessage);

    // Batching setup (reduce network overhead)
    constexpr size_t BATCH_SIZE     = 25;
    size_t           processedCount = 0;

    std::vector<std::array<char, 800>> batchMessages;
    batchMessages.reserve(BATCH_SIZE);

    // Process blocks
    std::cout << "block size" << blocks.size() << std::endl;
    for (const auto& block : blocks) {
        processBlock(block, batchMessages);
        processedCount++;
        if (batchMessages.size() == BATCH_SIZE) {
            processBatchMessages(batchMessages, "block");
        }
    }
    processBatchMessages(batchMessages, "block");

    // Process waves
    for (const auto& wave : waves) {
        processWave(wave, batchMessages);
        if (batchMessages.size() == BATCH_SIZE) {
            processBatchMessages(batchMessages, "wave");
        }
    }
    processBatchMessages(batchMessages, "wave");
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

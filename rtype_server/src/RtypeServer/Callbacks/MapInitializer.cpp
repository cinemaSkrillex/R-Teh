/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** MapInitializer
*/

#include "MapInitializer.hpp"

void MapInitializer::initializeMap(const asio::ip::udp::endpoint& sender) {
    if (!_gameInstance) {
        std::cerr << "Error: Game instance is null" << std::endl;
        return;
    }
    auto GameMap = _gameInstance->getMap();
    if (!GameMap) {
        std::cerr << "Error: Server map is null" << std::endl;
        return;
    }

    // Send blocks
    sendEntities(GameMap->getBlockEntities(), "block", sender);
    // sendEntities(GameMap->getWaves(), "wave", sender);

    // Serialize and send map info
    RTypeProtocol::MapMessage mapMessage           = createMapMessage(GameMap);
    std::array<char, 800>     serializedMapMessage = RTypeProtocol::serialize<800>(mapMessage);

    RTypeProtocol::BaseMessage baseMessage = RTypeProtocol::deserialize<800>(serializedMapMessage);

    RTypeProtocol::MapMessage deserializedMapMessage;

    switch (baseMessage.message_type) {
        case RTypeProtocol::MessageType::MAP_INFO:
            deserializedMapMessage =
                RTypeProtocol::deserializeMapMessage<800>(serializedMapMessage);
            std::cout << "Deserialized map message: " << deserializedMapMessage.message_type
                      << std::endl;
            std::cout << "Deserialized map message: " << deserializedMapMessage.uuid << std::endl;
            std::cout << "Deserialized map message: " << deserializedMapMessage.scrollingSpeed
                      << std::endl;
            std::cout << "Deserialized map message: " << deserializedMapMessage.x_level_position
                      << std::endl;
            std::cout << "Deserialized map message: " << deserializedMapMessage.isLoaded
                      << std::endl;
            std::cout << "Deserialized map message: " << deserializedMapMessage.isLevelRunning
                      << std::endl;
            std::cout << "Deserialized map message: " << deserializedMapMessage.server_tick
                      << std::endl;
            std::cout << "Deserialized map message: " << deserializedMapMessage.level_music.data()
                      << std::endl;
            std::cout << "Deserialized map message: " << deserializedMapMessage.level_music.size()
                      << std::endl;
            for (const auto& bg : deserializedMapMessage.backgrounds) {
                std::cout << "Deserialized map message: " << bg.data.data() << std::endl;
                std::cout << "Deserialized map message: " << bg.data.size() << std::endl;
                std::cout << "Deserialized map message: " << bg.speed << std::endl;
            }
            break;
        default:
            std::cout << "Unknown message type: " << baseMessage.message_type << std::endl;
            break;
    }
    _UdpServer->send_reliable_packet(serializedMapMessage, sender);
}

void MapInitializer::processBlock(const std::shared_ptr<rtype::Block>& block,
                                  std::vector<std::array<char, 800>>&  batchMessages) {
    auto blockEntity = block->getEntity();

    if (!blockEntity) {
        std::cerr << "Error: Block entity is null" << std::endl;
        return;
    }

    RTypeProtocol::NewEntityMessage newTileMessage;
    newTileMessage.message_type = RTypeProtocol::MessageType::NEW_ENTITY;
    newTileMessage.uuid         = *blockEntity;
    newTileMessage.entity_type  = RTypeProtocol::EntityType::BLOCK;

    auto& registry = _gameInstance->getRegistryRef();
    auto* position = registry.get_component<RealEngine::Position>(*blockEntity);
    auto* rotation = registry.get_component<RealEngine::Rotation>(*blockEntity);
    if (!position || position->x < 0) {
        std::cerr << "Error: Block position is null" << std::endl;
        return;
    }
    if (!rotation) {
        std::cerr << "Error: Block rotation is null" << std::endl;
        return;
    }
    addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::POSITION, *position);
    addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::ROTATION, *rotation);
    addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::DRAWABLE, true);

    sf::FloatRect bounds = {0, 0, 16, 8};
    addCollisionComponentToMessage(newTileMessage, bounds, block->getElement(), false,
                                   RealEngine::CollisionType::SOLID);

    std::string sprite = block->getElement();
    // std::cout << "Sprite: " << sprite << std::endl;
    std::vector<char> spriteData(sprite.begin(), sprite.end());
    addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::SPRITE, spriteData);

    std::array<char, 800> serializedMessage = RTypeProtocol::serialize<800>(newTileMessage);
    batchMessages.emplace_back(serializedMessage);
}

RTypeProtocol::MapMessage MapInitializer::createMapMessage(
    const std::shared_ptr<GameMap>& GameMap) {
    RTypeProtocol::MapMessage mapMessage;
    mapMessage.message_type     = RTypeProtocol::MessageType::MAP_INFO;
    mapMessage.uuid             = 0;
    mapMessage.scrollingSpeed   = GameMap->getScrollingSpeed();
    mapMessage.x_level_position = GameMap->getXLevelPosition();
    mapMessage.isLoaded         = GameMap->isLoaded();
    mapMessage.isLevelRunning   = GameMap->getIsLevelRunning();
    mapMessage.server_tick      = _serverConfig.getConfigItem<int>("SERVER_TICK");
    std::string musicName       = GameMap->getMusicName();

    mapMessage.level_music.assign(musicName.begin(), musicName.end());

    std::cout << "Level music: " << mapMessage.level_music.data() << std::endl;
    std::cout << "Level size : " << mapMessage.level_music.size() << std::endl;
    const auto& backgrounds = GameMap->getBackgrounds();
    for (const auto& background : backgrounds) {
        RTypeProtocol::BackgroundData bgData;
        bgData.data.assign(background.first.begin(), background.first.end());
        bgData.speed = background.second;
        std::cout << "Background: " << bgData.data.data() << ", Speed: " << bgData.speed
                  << std::endl;
        std::cout << "Background size: " << bgData.data.size() << std::endl;
        mapMessage.backgrounds.push_back(bgData);
    }
    return mapMessage;
}

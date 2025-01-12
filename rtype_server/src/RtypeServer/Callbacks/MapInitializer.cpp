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

    RTypeProtocol::MapMessage mapMessage;
    mapMessage.message_type     = RTypeProtocol::MessageType::MAP_INFO;
    mapMessage.uuid             = 0;
    mapMessage.scrollingSpeed   = GameMap->getScrollingSpeed();
    mapMessage.x_level_position = GameMap->getXLevelPosition();
    mapMessage.isLoaded         = GameMap->isLoaded();
    mapMessage.server_tick      = _serverConfig.getConfigItem<int>("SERVER_TICK");

    // Serialize and send map info

    // Send blocks
    sendEntities(GameMap->getBlockEntities(), "block", sender);
    // sendEntities(GameMap->getWaves(), "wave", sender);

    std::array<char, 800> serializedMapMessage = RTypeProtocol::serialize<800>(mapMessage);
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
    addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::BLOCKTAG, true);

    sf::FloatRect bounds = {0, 0, 16, 8};
    addCollisionComponentToMessage(newTileMessage, bounds, block->getElement(), false,
                                   RealEngine::CollisionType::SOLID);

    std::string sprite = block->getElement();
    std::cout << "Sprite: " << sprite << std::endl;
    std::vector<char> spriteData(sprite.begin(), sprite.end());
    addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::SPRITE, spriteData);

    std::array<char, 800> serializedMessage = RTypeProtocol::serialize<800>(newTileMessage);
    batchMessages.push_back(serializedMessage);
}

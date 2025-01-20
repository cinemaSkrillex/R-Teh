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
    // sendEntities(GameMap->getBlockEntities(), "block", sender);
    // sendEntities(GameMap->getWaves(), "wave", sender);
    RTypeProtocol::MapMessage mapMessage           = createMapMessage(GameMap);
    std::array<char, 800>     serializedMapMessage = RTypeProtocol::serialize<800>(mapMessage);

    _UdpServer->send_reliable_packet(serializedMapMessage, sender);
    // Serialize and send map info
}

void MapInitializer::processBlock(const std::shared_ptr<rtype::BaseBlock>& block,
                                  std::array<char, 800>&                   message) {
    auto blockEntity = block->getEntity();

    if (!blockEntity) {
        std::cerr << "Error: Block entity is null" << std::endl;
        return;
    }

    RTypeProtocol::NewEntityMessage newTileMessage;
    newTileMessage.message_type = RTypeProtocol::MessageType::NEW_ENTITY;
    newTileMessage.uuid         = *blockEntity;
    newTileMessage.entity_type  = RTypeProtocol::EntityType::BLOCK;
    std::cout << "Block entity: " << *blockEntity << std::endl;

    auto& registry      = _gameInstance->getRegistryRef();
    auto* position      = registry.get_component<RealEngine::Position>(*blockEntity);
    auto* rotation      = registry.get_component<RealEngine::Rotation>(*blockEntity);
    auto* Interpolation = registry.get_component<RealEngine::Interpolation>(*blockEntity);
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
    // addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::INTERPOLATION,
    //                       *Interpolation);

    sf::FloatRect bounds = {0, 0, 16, 8};
    addCollisionComponentToMessage(newTileMessage, bounds, block->getElement(), false,
                                   RealEngine::CollisionType::SOLID);

    std::string sprite = block->getElement();
    // std::cout << "Sprite: " << sprite << std::endl;
    std::vector<char> spriteData(sprite.begin(), sprite.end());
    addComponentToMessage(newTileMessage, RTypeProtocol::ComponentList::SPRITE, spriteData);

    std::array<char, 800> serializedMessage = RTypeProtocol::serialize<800>(newTileMessage);
    message                                 = serializedMessage;
}

RTypeProtocol::MapMessage MapInitializer::createMapMessage(
    const std::shared_ptr<ServerMap>& GameMap) {
    RTypeProtocol::MapMessage mapMessage;
    mapMessage.message_type     = RTypeProtocol::MessageType::MAP_INFO;
    mapMessage.uuid             = 0;
    mapMessage.scrollingSpeed   = GameMap->getScrollingSpeed();
    mapMessage.x_level_position = GameMap->getXLevelPosition();
    mapMessage.isLoaded         = GameMap->isLoaded();
    mapMessage.isLevelRunning   = GameMap->getIsLevelRunning();
    std::cout << "map scrolling speed: " << mapMessage.scrollingSpeed << std::endl;
    std::cout << "map x level position: " << mapMessage.x_level_position << std::endl;
    std::cout << "Map is loaded: " << mapMessage.isLoaded << std::endl;
    std::cout << "Map is running: " << mapMessage.isLevelRunning << std::endl;
    mapMessage.server_tick = _serverConfig.getConfigItem<int>("SERVER_TICK");
    std::string musicName  = GameMap->getMusicName();
    if (musicName.empty()) {
        std::cerr << "Error: Music name is empty" << std::endl;
        return mapMessage;
    }
    if (musicName == "level_1") {
        mapMessage.id_level_music = 1;
    } else if (musicName == "level_2") {
        mapMessage.id_level_music = 2;
    } else if (musicName == "level_3") {
        mapMessage.id_level_music = 3;
    } else if (musicName == "waiting_room") {
        mapMessage.id_level_music = 4;
    } else {
        mapMessage.id_level_music = 1;
    }

    const auto& backgrounds = GameMap->getBackgrounds();
    std::cout << "backgrounds size: " << backgrounds.size() << std::endl;
    for (const auto& background : backgrounds) {
        RTypeProtocol::BackgroundData bgData;
        if (background.first.empty()) {
            std::cerr << "Error: Background data is empty" << std::endl;
            continue;
        }
        if (background.first == "big_stars_background") {
            bgData.background_id = 1;
        } else if (background.first == "medium_stars_background") {
            bgData.background_id = 2;
        } else if (background.first == "small_stars_background") {
            bgData.background_id = 3;
        } else if (background.first == "space_base_background") {
            bgData.background_id = 4;
        } else if (background.first == "front_line_base") {
            bgData.background_id = 5;
        } else {
            bgData.background_id = 1;
        }
        bgData.speed = background.second;
        mapMessage.backgrounds.push_back(bgData);
        std::cout << "map message backgrounds size: " << mapMessage.backgrounds.size() << std::endl;
    }
    return mapMessage;
}

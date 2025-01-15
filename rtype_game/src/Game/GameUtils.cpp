/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** GameUtils
*/

#include "Game/Game.hpp"

void rtype::Game::handleSignal(std::array<char, 800> signal) {
    // Check if signal is empty or corrupted (all zeros)
    if (std::all_of(signal.begin(), signal.end(), [](char c) { return c == 0; })) {
        return;
    }

    // Deserialize the signal into a BaseMessage first
    RTypeProtocol::BaseMessage baseMessage = RTypeProtocol::deserialize<800>(signal);
    // Print the deserialized BaseMessage
    // Check the message type to handle accordingly
    switch (baseMessage.message_type) {
        case RTypeProtocol::NEW_CLIENT: {
            // Deserialize the PlayerMoveMessage (which is used for new client)
            RTypeProtocol::PlayerMoveMessage newClientMessage =
                RTypeProtocol::deserializePlayerMove(signal);

            handleNewClient(newClientMessage);
            break;
        }
        case RTypeProtocol::SYNCHRONISE: {
            RTypeProtocol::SynchronizeMessage syncMessage =
                RTypeProtocol::deserializeSynchronize(signal);
            handleSynchronize(syncMessage);
            break;
        }
        case RTypeProtocol::MAP_INFO: {
            RTypeProtocol::MapMessage mapMessage = RTypeProtocol::deserializeMapMessage(signal);
            handleMapMessage(mapMessage);
            break;
        }
        case RTypeProtocol::PLAYER_MOVE: {
            // Deserialize and handle player movement
            RTypeProtocol::PlayerMoveMessage playerMoveMessage =
                RTypeProtocol::deserializePlayerMove(signal);
            handlePlayerMove(playerMoveMessage);
            break;
        }
        case RTypeProtocol::NEW_ENTITY: {
            RTypeProtocol::NewEntityMessage newEntityMessage =
                RTypeProtocol::deserializeNewEntityMessage(signal);
            handleNewEntity(newEntityMessage);
            break;
        }
        case RTypeProtocol::DESTROY_ENTITY: {
            // Deserialize and handle destroy entity message
            RTypeProtocol::DestroyEntityMessage destroyEntityMessage =
                RTypeProtocol::deserializeDestroyEntity(signal);
            handleDestroyEntity(destroyEntityMessage);
            break;
        }
        case RTypeProtocol::LEVEL_SIGNAL: {
            RTypeProtocol::LevelSignalMessage levelSignalMessage =
                RTypeProtocol::deserializeLevelSignal(signal);
            if (levelSignalMessage.startLevel) {
                _game_map.startLevel();
            } else {
                _game_map.stopLevel();
            }
            break;
        }
        case RTypeProtocol::ENTITY_UPDATE: {
            RTypeProtocol::EntityUpdateMessage entityUpdateMessage =
                RTypeProtocol::deserializeEntityUpdate(signal);
            handleEntityUpdate(entityUpdateMessage);
            break;
        }
        case RTypeProtocol::PLAYER_UPDATE_SCORE: {
            RTypeProtocol::OneIntMessage playerUpdateScoreMessage =
                RTypeProtocol::deserializeOneIntMessage(signal);
            std::cout << "Player score: " << playerUpdateScoreMessage.value << std::endl;
            break;
        }
        case RTypeProtocol::PLAYER_UPDATE_HEALTH: {
            RTypeProtocol::OneIntMessage playerUpdateHealthMessage =
                RTypeProtocol::deserializeOneIntMessage(signal);
            std::cout << "Player health: " << playerUpdateHealthMessage.value << std::endl;
            break;
        }

        default:
            // Handle unknown or unsupported message types (you can log or handle errors)
            std::cout << "Unknown message type: " << baseMessage.message_type << std::endl;
            break;
    }
}

void rtype::Game::handleNewClient(RTypeProtocol::PlayerMoveMessage parsedPacket) {
    const sf::Vector2f position = {parsedPacket.x, parsedPacket.y};
    const long int     uuid     = parsedPacket.uuid;
    if (_players.find(uuid) != _players.end()) return;
    add_player(uuid, position);
}

void rtype::Game::handleSynchronize(RTypeProtocol::SynchronizeMessage parsedPacket) {
    _localPlayerUUID            = parsedPacket.uuid;
    _serverTime                 = parsedPacket.timestamp;
    auto client_now             = std::chrono::steady_clock::now();
    _startTime                  = client_now - std::chrono::milliseconds(_serverTime);
    sf::Vector2f localPlayerPos = {parsedPacket.x, parsedPacket.y};
    _registry.add_component(_player_entity,
                            RealEngine::Position{localPlayerPos.x, localPlayerPos.y});
    for (const auto& player : parsedPacket.players) {
        add_player(player.first, player.second);
    }
}

void rtype::Game::handlePlayerMove(RTypeProtocol::PlayerMoveMessage parsedPacket) {
    const long int     uuid     = parsedPacket.uuid;
    const sf::Vector2f position = {parsedPacket.x, parsedPacket.y};
    float              step     = parsedPacket.step;
    auto               it       = _players.find(uuid);

    if (it == _players.end()) return;
    std::shared_ptr<RealEngine::Entity> player = it->second;
    // Update the position component of the player
    auto* player_sprite          = _registry.get_component<RealEngine::SpriteSheet>(player);
    auto* positionComponent      = _registry.get_component<RealEngine::Position>(player);
    auto* interpolationComponent = _registry.get_component<RealEngine::Interpolation>(player);
    if (!positionComponent && !interpolationComponent) return;
    if ((position.y < positionComponent->y) && (std::abs(position.y - positionComponent->y) > 7)) {
        player_sprite->spriteIndex = "up";
    } else if ((position.y > positionComponent->y) &&
               (std::abs(position.y - positionComponent->y) > 7)) {
        player_sprite->spriteIndex = "down";
    } else {
        player_sprite->spriteIndex = "idle";
    }
    positionComponent->x                 = interpolationComponent->end.x;
    positionComponent->y                 = interpolationComponent->end.y;
    interpolationComponent->start        = {positionComponent->x, positionComponent->y};
    interpolationComponent->end          = position;
    interpolationComponent->step         = 1.f / step;
    interpolationComponent->current_step = 0.f;
    interpolationComponent->reset        = true;
}

void rtype::Game::handleEntityUpdate(RTypeProtocol::EntityUpdateMessage parsedPacket) {
    auto it       = _entities.find(parsedPacket.uuid);
    auto playerIt = _players.find(parsedPacket.uuid);
    if (playerIt != _players.end()) return;
    if (it == _entities.end()) return;
    std::shared_ptr<RealEngine::Entity> entity = it->second;
    auto* positionComponent = _registry.get_component<RealEngine::Position>(entity);
    auto* rotationComponent = _registry.get_component<RealEngine::Rotation>(entity);
    // auto* interpolationComponent              =
    // _registry.get_component<RealEngine::Interpolation>(entity); if (!positionComponent &&
    // !interpolationComponent) return;
    if (!positionComponent) return;
    // positionComponent->x                 = interpolationComponent->end.x;
    // positionComponent->y                 = interpolationComponent->end.y;
    positionComponent->x = parsedPacket.x;
    positionComponent->y = parsedPacket.y;

    // interpolationComponent->start        = {positionComponent->x, positionComponent->y};
    // interpolationComponent->end          = {parsedPacket.x, parsedPacket.y};
    // interpolationComponent->step         = parsedPacket.step;
    // interpolationComponent->current_step = 0.f;
    // interpolationComponent->reset        = true;
    if (rotationComponent && parsedPacket.angle != -1) {
        rotationComponent->angle = parsedPacket.angle;
    }
}

void rtype::Game::handleNewEntity(RTypeProtocol::NewEntityMessage parsedPacket) {
    auto newEntity = _registry.spawn_entity();
    for (const auto& component : parsedPacket.components) {
        switch (component.first) {
            case RTypeProtocol::ComponentList::POSITION: {
                RealEngine::Position position;
                std::memcpy(&position, component.second.data(), sizeof(position));
                // std::cout << "Position: (" << position.x << ", " << position.y
                //           << "entity : " << parsedPacket.uuid << "newEntity" << *newEntity <<
                //           ")\n";
                _registry.add_component(newEntity, RealEngine::Position{position.x, position.y});
                break;
            }
            case RTypeProtocol::ComponentList::VELOCITY: {
                RealEngine::Velocity velocity;
                std::memcpy(&velocity, component.second.data(), sizeof(velocity));
                // std::cout << "Velocity: (" << velocity.vx << ", " << velocity.vy << "),
                // MaxSpeed:
                // ("
                //           << velocity.maxSpeed.x << ", " << velocity.maxSpeed.y
                //           << "), AirFrictionForce: " << velocity.airFrictionForce << "\n";
                _registry.add_component(
                    newEntity, RealEngine::Velocity{velocity.vx, velocity.vy, velocity.maxSpeed,
                                                    velocity.airFrictionForce});
                break;
            }
            case RTypeProtocol::ComponentList::SPRITE: {
                std::string sprite_str(component.second.begin(), component.second.end());
                // std::cout << "Sprite: " << sprite_str << "\n";

                auto spriteSheet =
                    RealEngine::AssetManager::getInstance().getSpriteSheet(sprite_str);
                if (spriteSheet) {
                    _registry.add_component(*newEntity, RealEngine::SpriteSheet{*spriteSheet});
                } else {
                    auto sprite = RealEngine::AssetManager::getInstance().getSprite(sprite_str);
                    if (sprite) {
                        _registry.add_component(
                            *newEntity,
                            RealEngine::SpriteComponent{
                                *sprite,
                                parsedPacket.entity_type == RTypeProtocol::EntityType::BLOCK ? 1
                                                                                             : 0});
                    } else {
                        std::cerr << "Failed to load Sprite or SpriteSheet for ID: " << sprite_str
                                  << std::endl;
                    }
                }
                break;
            }

            case RTypeProtocol::ComponentList::ROTATION: {
                RealEngine::Rotation rotation;
                std::memcpy(&rotation, component.second.data(), sizeof(rotation));
                // std::cout << "Rotation: " << rotation.angle << "\n";
                _registry.add_component(newEntity, RealEngine::Rotation{rotation.angle});
                break;
            }
            case RTypeProtocol::ComponentList::COLLISION: {
                // TODO: handle collision client side
                break;
            }
            case RTypeProtocol::ComponentList::AUTO_DESTRUCTIBLE: {
                float autoDestructible;
                std::memcpy(&autoDestructible, component.second.data(), sizeof(autoDestructible));
                // std::cout << "AutoDestructible: " << autoDestructible << "\n";
                _registry.add_component<RealEngine::AutoDestructible>(
                    *newEntity, RealEngine::AutoDestructible{autoDestructible});
                break;
            }
            case RTypeProtocol::ComponentList::DRAWABLE: {
                // std::cout << "Drawable: " << std::endl;
                _registry.add_component<RealEngine::Drawable>(*newEntity, RealEngine::Drawable{});
                break;
            }
            case RTypeProtocol::ComponentList::ACCELERATION: {
                RealEngine::Acceleration acceleration;
                std::memcpy(&acceleration, component.second.data(), sizeof(acceleration));
                _registry.add_component(newEntity, RealEngine::Acceleration{acceleration});
                break;
            }
            default:
                std::cout << "Unknown component type: " << component.first << "\n";
                break;
        }
    }
    if (_entities.find(parsedPacket.uuid) != _entities.end()) {
        std::cout << "Entity with UUID " << parsedPacket.uuid << " already exists, skipping."
                  << std::endl;
        return;
    }
    addEntityToGame(parsedPacket, newEntity);
}

void rtype::Game::handleDestroyEntity(RTypeProtocol::DestroyEntityMessage parsedPacket) {
    for (const auto& entity_id : parsedPacket.entity_ids) {
        auto it = _entities.find(entity_id);
        if (it != _entities.end()) {
            _registry.remove_entity(*it->second);
            _entities.erase(it);
            continue;
        }
        auto playerIt = _players.find(entity_id);
        if (playerIt != _players.end()) {
            _registry.remove_entity(*playerIt->second);
            _players.erase(playerIt);
        }
        if (_localPlayerUUID == entity_id) {
            _registry.remove_entity(*_player_entity);
            _player_entity.reset();
        }
    }
}

void rtype::Game::handleMapMessage(RTypeProtocol::MapMessage parsedPacket) {
    _game_map.setScrollingSpeed(parsedPacket.scrollingSpeed);
    _game_map.setXLevelPosition(parsedPacket.x_level_position);
    _game_map.setIsMapLoaded(parsedPacket.isLoaded);
    _serverTick = parsedPacket.server_tick;
    std::cout << "Received map info: ScrollingSpeed: " << _game_map.getScrollingSpeed()
              << ", XLevelPosition: " << _game_map.getXLevelPosition()
              << ", isLoaded: " << _game_map.isMapLoaded() << " levelRunning"
              << _game_map.levelRunning() << ", ServerTick: " << _serverTick << std::endl;

    if (parsedPacket.id_level_music != -1) {
        std::string level_music_str;
        switch (parsedPacket.id_level_music) {
            case 1:
                level_music_str = "level_1";
                break;
            case 2:
                level_music_str = "level_2";
                break;
            case 3:
                level_music_str = "level_3";
                break;
            default:
                level_music_str = "level_1";
                break;
        }
        _game_map.setMusicName(level_music_str);
    }

    for (const auto& bg : parsedPacket.backgrounds) {
        std::string backgroundStr;
        switch (bg.background_id) {
            case 1:
                backgroundStr = "big_stars_background";
                break;
            case 2:
                backgroundStr = "medium_stars_background";
                break;
            case 3:
                backgroundStr = "small_stars_background";
                break;
            case 4:
                backgroundStr = "space_base_background";
                break;
            default:
                backgroundStr = "big_stars_background";
                break;
        }
        Background background(_registry, bg.speed, backgroundStr);
        _game_map.addBackground(background.getEntity(), _parallaxSystem);
    }

    if (_game_map.levelRunning() == false && parsedPacket.isLevelRunning == true) {
        _game_map.startLevel();
        _game_map.synchroniseLevelBlockEntities();
    } else if (_game_map.levelRunning() == true && parsedPacket.isLevelRunning == false) {
        _game_map.stopLevel();
        _game_map.synchroniseLevelBlockEntities();
    }
}

void rtype::Game::addEntityToGame(RTypeProtocol::NewEntityMessage     parsedPacket,
                                  std::shared_ptr<RealEngine::Entity> newEntity) {
    switch (parsedPacket.entity_type) {
        case RTypeProtocol::EntityType::BLOCK:
            if (std::find_if(
                    _game_map.getBlockEntities().begin(), _game_map.getBlockEntities().end(),
                    [&parsedPacket](
                        const std::pair<long int, std::shared_ptr<RealEngine::Entity>>& block) {
                        return block.first == parsedPacket.uuid;
                    }) != _game_map.getBlockEntities().end()) {
                std::cerr << "Block entity with UUID " << parsedPacket.uuid
                          << " already exists in the game map, skipping." << std::endl;
                _registry.remove_entity(*newEntity);
                return;
            }
            // std::cout << "Adding block with UUID: " << parsedPacket.uuid << "Entity: " <<
            // *newEntity
            //           << std::endl;
            _game_map.addBlock(newEntity, parsedPacket.uuid);
            break;
        case RTypeProtocol::EntityType::OTHER_ENTITY:
            _entities.emplace(parsedPacket.uuid, newEntity);
            break;
        default:
            break;
    }
}
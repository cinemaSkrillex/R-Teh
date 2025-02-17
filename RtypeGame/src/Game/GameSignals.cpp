/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** GameUtils
*/

#include <utility>

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
    switch (baseMessage.messageType) {
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
            if (levelSignalMessage.startLevel && _gameMap) {
                _gameMap->startLevel();
            } else {
                _gameMap->stopLevel();
            }
            break;
        }
        case RTypeProtocol::ENTITY_UPDATE: {
            RTypeProtocol::EntityUpdateMessage entityUpdateMessage =
                RTypeProtocol::deserializeEntityUpdate(signal);
            handleEntityUpdate(entityUpdateMessage);
            break;
        }
        case RTypeProtocol::PLAYER_UPDATE_DATA: {
            RTypeProtocol::PlayerUpdateDataMessage playerUpdateDataMessage =
                RTypeProtocol::deserializePlayerUpdateDataMessage(signal);
            handlePlayerValues(playerUpdateDataMessage);
            break;
        }
        case RTypeProtocol::CHANGING_SCENE: {
            RTypeProtocol::ChangingSceneMessage changingSceneMessage =
                RTypeProtocol::deserializeChangingSceneMessage(signal);
            handleChangingScene(changingSceneMessage);
            break;
        }
        default:
            // Handle unknown or unsupported message types (you can log or handle errors)
            std::cout << "Unknown message type: " << baseMessage.messageType << std::endl;
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
    _registry.addComponent(_playerEntity, RealEngine::Position{localPlayerPos.x, localPlayerPos.y});
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
    auto* player_sprite          = _registry.getComponent<RealEngine::SpriteSheet>(player);
    auto* positionComponent      = _registry.getComponent<RealEngine::Position>(player);
    auto* interpolationComponent = _registry.getComponent<RealEngine::Interpolation>(player);
    if (!positionComponent && !interpolationComponent) return;
    if ((position.y < positionComponent->y) && (std::abs(position.y - positionComponent->y) > 7)) {
        player_sprite->spriteIndex = "up";
    } else if ((position.y > positionComponent->y) &&
               (std::abs(position.y - positionComponent->y) > 7)) {
        player_sprite->spriteIndex = "down";
    } else {
        player_sprite->spriteIndex = "idle";
    }
    positionComponent->x                = interpolationComponent->end.x;
    positionComponent->y                = interpolationComponent->end.y;
    interpolationComponent->start       = {positionComponent->x, positionComponent->y};
    interpolationComponent->end         = position;
    interpolationComponent->step        = 1.f / step;
    interpolationComponent->currentStep = 0.f;
    interpolationComponent->reset       = true;
}

void rtype::Game::handlePlayerValues(RTypeProtocol::PlayerUpdateDataMessage parsedPacket) {
    auto* position        = _registry.getComponent<RealEngine::Position>(_playerEntity);
    auto* healthComponent = _registry.getComponent<RealEngine::Health>(_playerEntity);
    if (healthComponent) {
        auto healthDiff = healthComponent->amount - parsedPacket.health;
        if (healthDiff > 0 && position) {
            RealEngine::AssetManager::getInstance().getSound("hit_hurt")->play();
            HitEffect(_registry, {position->x, position->y});
            auto* container = _registry.getComponent<RealEngine::NetvarContainer>(_playerEntity);
            if (container && container->getNetvar("clignotingTimer")) {
                container->getNetvar("clignotingTimer")->value = 1.5f;
            }
            displayTemporaryText(std::to_string(healthDiff), {position->x, position->y},
                                 sf::Color::Red, 20);
        } else if (healthDiff < 0 && position) {
            RealEngine::AssetManager::getInstance().getSound("powerup_heal")->play();
            displayTemporaryText(std::to_string(abs(healthDiff)), {position->x, position->y},
                                 sf::Color::Green, 20);
        }
        healthComponent->amount = parsedPacket.health;
    }
    auto* scoreComponent = _registry.getComponent<RealEngine::Score>(_playerEntity);
    if (scoreComponent) {
        auto scoreDiff = parsedPacket.score - scoreComponent->amount;
        if (scoreDiff > 0 && position) {
            displayTemporaryText(std::to_string(scoreDiff), {position->x, position->y},
                                 sf::Color::Yellow, 20);
        }
        scoreComponent->amount = parsedPacket.score;
    }
}

void rtype::Game::handleEntityUpdate(RTypeProtocol::EntityUpdateMessage parsedPacket) {
    auto it       = _entities.find(parsedPacket.uuid);
    auto playerIt = _players.find(parsedPacket.uuid);
    if (playerIt != _players.end()) return;
    if (it == _entities.end()) return;
    std::shared_ptr<RealEngine::Entity> entity = it->second.entity;
    auto* positionComponent      = _registry.getComponent<RealEngine::Position>(entity);
    auto* rotationComponent      = _registry.getComponent<RealEngine::Rotation>(entity);
    auto* interpolationComponent = _registry.getComponent<RealEngine::Interpolation>(entity);
    if (rotationComponent && parsedPacket.angle != -1) {
        rotationComponent->angle = parsedPacket.angle;
    }
    if (interpolationComponent) {
        positionComponent->x                = interpolationComponent->end.x;
        positionComponent->y                = interpolationComponent->end.y;
        interpolationComponent->start       = {positionComponent->x, positionComponent->y};
        interpolationComponent->end         = {parsedPacket.x, parsedPacket.y};
        interpolationComponent->step        = 1.f / parsedPacket.step;
        interpolationComponent->currentStep = 0.f;
        interpolationComponent->reset       = true;
    } else {
        if (!positionComponent) return;
        positionComponent->x = parsedPacket.x;
        positionComponent->y = parsedPacket.y;
    }
}

void rtype::Game::handleNewEntity(RTypeProtocol::NewEntityMessage parsedPacket) {
    // if (_entities.find(parsedPacket.uuid) != _entities.end()) {
    //     std::cout << "Entity with UUID " << parsedPacket.uuid << " already exists, skipping."
    //               << std::endl;
    //     return;
    // }
    auto newEntity = _registry.spawnEntity();
    for (const auto& component : parsedPacket.components) {
        switch (component.first) {
            case RTypeProtocol::ComponentList::POSITION: {
                RealEngine::Position position;
                std::memcpy(&position, component.second.data(), sizeof(position));
                // std::cout << "Position: (" << position.x << ", " << position.y << ")\n";
                _registry.addComponent(newEntity, RealEngine::Position{position.x, position.y});
                break;
            }
            case RTypeProtocol::ComponentList::INTERPOLATION: {
                RealEngine::Interpolation interpolation;
                std::memcpy(&interpolation, component.second.data(), sizeof(interpolation));
                _registry.addComponent(newEntity, RealEngine::Interpolation{interpolation});
                break;
            }
            case RTypeProtocol::ComponentList::VELOCITY: {
                RealEngine::Velocity velocity;
                std::memcpy(&velocity, component.second.data(), sizeof(velocity));
                _registry.addComponent(
                    newEntity, RealEngine::Velocity{velocity.vx, velocity.vy, velocity.maxSpeed,
                                                    velocity.airFrictionForce});
                break;
            }
            case RTypeProtocol::ComponentList::SPRITE: {
                std::string sprite_str(component.second.begin(), component.second.end());
                std::cout << "Sprite: " << sprite_str << "entity " << *newEntity << std::endl;

                auto spriteSheet =
                    RealEngine::AssetManager::getInstance().getSpriteSheet(sprite_str);
                if (spriteSheet) {
                    _registry.addComponent(*newEntity, RealEngine::SpriteSheet{*spriteSheet});
                } else {
                    // if (sprite) {
                    //     _registry.addComponent(
                    //         *newEntity,
                    //         RealEngine::SpriteComponent{
                    //             *sprite,
                    //             parsedPacket.entity_type == RTypeProtocol::EntityType::BLOCK ? 1
                    //                                                                          :
                    //                                                                          0});
                    auto sprite = RealEngine::AssetManager::getInstance().getSprite(sprite_str);
                    if (sprite) {
                        _registry.addComponent(*newEntity, RealEngine::SpriteComponent{*sprite, 0});
                        // parsedPacket.entity_type == RTypeProtocol::EntityType::BLOCK ? 1
                        //                                                              : 0});
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
                _registry.addComponent(newEntity, RealEngine::Rotation{rotation.angle});
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
                _registry.addComponent<RealEngine::AutoDestructible>(
                    *newEntity, RealEngine::AutoDestructible{autoDestructible});
                break;
            }
            case RTypeProtocol::ComponentList::DRAWABLE: {
                // std::cout << "Drawable: " << std::endl;
                _registry.addComponent<RealEngine::Drawable>(*newEntity, RealEngine::Drawable{});
                break;
            }
            case RTypeProtocol::ComponentList::ACCELERATION: {
                RealEngine::Acceleration acceleration;
                std::memcpy(&acceleration, component.second.data(), sizeof(acceleration));
                _registry.addComponent(newEntity, RealEngine::Acceleration{acceleration});
                break;
            }
            default:
                std::cout << "Unknown component type: " << component.first << "\n";
                break;
        }
    }
    addEntityToGame(parsedPacket, newEntity);
}

void rtype::Game::handleDestroyEntity(RTypeProtocol::DestroyEntityMessage parsedPacket) {
    for (const auto& entity_id : parsedPacket.entity_ids) {
        auto it = _entities.find(entity_id);
        if (it != _entities.end()) {
            if (_registry.isValid(*it->second.entity)) {
                _registry.removeEntity(*it->second.entity);
                _entities.erase(it);
            } else {
                _entities.erase(it);
            }
            continue;
        }
        auto playerIt = _players.find(entity_id);
        if (playerIt != _players.end()) {
            auto* position = _registry.getComponent<RealEngine::Position>(*playerIt->second);
            Explosion(_registry, {position->x, position->y});
            RealEngine::AssetManager::getInstance().getSound("explosion")->play();
            if (_registry.isValid(*playerIt->second)) {
                _registry.removeEntity(*playerIt->second);
                _players.erase(playerIt);
            } else {
                _players.erase(playerIt);
            }
        }
        if (_localPlayerUUID == entity_id) {
            if (_registry.isValid(*_playerEntity)) {
                _registry.removeEntity(*_playerEntity);
                _playerEntity.reset();
                return;
            }
            auto* position = _registry.getComponent<RealEngine::Position>(*_playerEntity);
            if (position) {
                Explosion(_registry, {position->x, position->y});
                RealEngine::AssetManager::getInstance().getSound("explosion")->play();
            }
            if (_playerEntity == nullptr) return;
            auto* health = _registry.getComponent<RealEngine::Health>(*_playerEntity);
            if (health) {
                health->amount = 0;
            }
            if (_registry.isValid(*_playerEntity)) {
                _registry.removeEntity(*_playerEntity);
                _playerEntity.reset();
            }
        }
    }
}

void rtype::Game::handleMapMessage(RTypeProtocol::MapMessage parsedPacket) {
    if (!_gameMap) {
        return;
    }
    _gameMap->setScrollingSpeed(parsedPacket.scrollingSpeed);
    _gameMap->setXLevelPosition(parsedPacket._xLevelPosition);
    _gameMap->setIsMapLoaded(parsedPacket.isLoaded);
    _serverTick = parsedPacket.server_tick;
    std::cout << "Received map info: ScrollingSpeed: " << _gameMap->getScrollingSpeed()
              << ", XLevelPosition: " << _gameMap->getXLevelPosition()
              << ", isLoaded: " << _gameMap->isMapLoaded() << " levelRunning"
              << _gameMap->levelRunning() << ", ServerTick: " << _serverTick << std::endl;

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
            case 4:
                level_music_str = "waiting_room";
                break;
            default:
                level_music_str = "level_1";
                break;
        }
        _gameMap->setMusicName(level_music_str);
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
            case 5:
                backgroundStr = "front_line_base";
                break;
            default:
                backgroundStr = "big_stars_background";
                break;
        }
        Background background(_registry, bg.speed, backgroundStr);
        std::cout << "Adding background: " << backgroundStr << std::endl;
        _gameMap->addBackground(background.getEntity(), _parallaxSystem);
    }

    if (_gameMap->levelRunning() == false && parsedPacket.isLevelRunning == true) {
        _gameMap->startLevel();
        _gameMap->synchroniseLevelBlockEntities();
    } else if (_gameMap->levelRunning() == true && parsedPacket.isLevelRunning == false) {
        _gameMap->stopLevel();
        _gameMap->synchroniseLevelBlockEntities();
    }
}

void rtype::Game::addEntityToGame(RTypeProtocol::NewEntityMessage     parsedPacket,
                                  std::shared_ptr<RealEngine::Entity> newEntity) {
    // _entities.emplace(parsedPacket.uuid, newEntity);
    RTypeProtocol::EntityType entityType = RTypeProtocol::EntityType::OTHER_ENTITY;
    std::cout << "BEFORE Entity type: " << static_cast<int>(entityType) << std::endl;
    if (parsedPacket.entity_type < 0 || parsedPacket.entity_type > 3) {
        std::cerr << "Invalid entity type: " << parsedPacket.entity_type << std::endl;
        entityType = RTypeProtocol::EntityType::OTHER_ENTITY;
    } else if (parsedPacket.entity_type == RTypeProtocol::EntityType::BLOCK) {
        entityType = RTypeProtocol::EntityType::BLOCK;
    }

    std::cout << "AFTER Entity type: " << static_cast<int>(entityType) << std::endl;

    // Store the entity and its type in the _entities map
    _entities.emplace(parsedPacket.uuid, EntityInfo{std::move(newEntity), entityType});
}

void rtype::Game::handleChangingScene(RTypeProtocol::ChangingSceneMessage parsedPacket) {
    std::cout << "Changing scene to: " << static_cast<int>(parsedPacket.scene_id) << std::endl;
    // relocateAllBlocks();
    unloadLevel(10000.0f, 10000.0f);
    RTypeProtocol::BaseMessage baseMessage;
    baseMessage.messageType                     = RTypeProtocol::MessageType::MAP_UNLOADED;
    baseMessage.uuid                            = 0;
    std::array<char, 800> serializedBaseMessage = RTypeProtocol::serialize<800>(baseMessage);
    _clientUDP->sendReliablePacket(serializedBaseMessage);
}
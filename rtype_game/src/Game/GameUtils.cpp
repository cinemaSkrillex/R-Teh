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
    auto* positionComponent      = _registry.get_component<RealEngine::Position>(player);
    auto* interpolationComponent = _registry.get_component<RealEngine::Interpolation>(player);
    if (!positionComponent && !interpolationComponent) return;
    positionComponent->x                 = interpolationComponent->end.x;
    positionComponent->y                 = interpolationComponent->end.y;
    interpolationComponent->start        = {positionComponent->x, positionComponent->y};
    interpolationComponent->end          = position;
    interpolationComponent->step         = 1.f / step;
    interpolationComponent->current_step = 0.f;
    interpolationComponent->reset        = true;
}

void rtype::Game::handleNewEntity(RTypeProtocol::NewEntityMessage parsedPacket) {
    auto newEntity = _registry.spawn_entity();
    for (const auto& component : parsedPacket.components) {
        switch (component.first) {
            case RTypeProtocol::ComponentList::POSITION: {
                RealEngine::Position position;
                std::memcpy(&position, component.second.data(), sizeof(position));
                // std::cout << "Position: (" << position.x << ", " << position.y << ")\n";
                _registry.add_component(newEntity, RealEngine::Position{position.x, position.y});
                break;
            }
            case RTypeProtocol::ComponentList::VELOCITY: {
                RealEngine::Velocity velocity;
                std::memcpy(&velocity, component.second.data(), sizeof(velocity));
                // std::cout << "Velocity: (" << velocity.vx << ", " << velocity.vy << "), MaxSpeed:
                // ("
                //           << velocity.maxSpeed.x << ", " << velocity.maxSpeed.y
                //           << "), AirFrictionForce: " << velocity.airFrictionForce << "\n";
                _registry.add_component(
                    newEntity, RealEngine::Velocity{velocity.vx, velocity.vy, velocity.maxSpeed,
                                                    velocity.airFrictionForce});
                break;
            }
            case RTypeProtocol::ComponentList::SPRITE: {
                // auto spriteSheet = *(RealEngine::AssetManager::getInstance().getSpriteSheet(sprite_str));
                // //check if spriteSheet is not null
                // // if not null do:
                // // _registry.add_component(*newEntity, RealEngine::SpriteSheet{spriteSheet});
                // //else
                // auto sprite = *(RealEngine::AssetManager::getInstance().getSprite(sprite_str));
                // _registry.add_component(*newEntity, RealEngine::SpriteComponent{sprite});
                // break;
                std::string sprite_str(component.second.begin(), component.second.end());
                // std::cout << "Sprite: " << sprite_str << "\n";
    
                auto spriteSheet = RealEngine::AssetManager::getInstance().getSpriteSheet(sprite_str);
                if (spriteSheet) {
                    _registry.add_component(*newEntity, RealEngine::SpriteSheet{*spriteSheet});
                } else {
                    auto sprite = RealEngine::AssetManager::getInstance().getSprite(sprite_str);
                    if (sprite) {
                        _registry.add_component(*newEntity, RealEngine::SpriteComponent{*sprite});
                    } else {
                        std::cerr << "Failed to load Sprite or SpriteSheet for ID: " << sprite_str << std::endl;
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
                //TODO: handle collision client side
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

            default:
                std::cout << "Unknown component type: " << component.first << "\n";
                break;
        }
    }
    // std::cout << "New entity created with UUID: " << parsedPacket.uuid << std::endl;
    _entities.emplace(parsedPacket.uuid, newEntity);
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

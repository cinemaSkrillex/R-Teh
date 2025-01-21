/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongGameSignals.cpp
*/

#include "../include/Game/PongGame.hpp"

void pong::Game::handleSignal(std::array<char, 800> signal) {
    if (std::all_of(signal.begin(), signal.end(), [](char c) { return c == 0; })) {
        return;
    }
    PongProtocol::BaseMessage baseMessage = PongProtocol::deserialize<800>(signal);
    switch (baseMessage.messageType) {
        case PongProtocol::PLAYER_MOVE: {
            PongProtocol::PlayerMoveMessage playerMoveMessage =
                PongProtocol::deserializePlayerMove(signal);
            handlePlayerMove(playerMoveMessage);
            break;
        }
        case PongProtocol::NEW_ENTITY: {
            PongProtocol::NewEntityMessage newEntityMessage =
                PongProtocol::deserializeNewEntity(signal);
            handleNewEntity(newEntityMessage);
            break;
        }
        case PongProtocol::ENTITY_UPDATE: {
            PongProtocol::EntityUpdateMessage entityUpdateMessage =
                PongProtocol::deserializeEntityUpdate(signal);
            handleEntityUpdate(entityUpdateMessage);
            break;
        }
        case PongProtocol::PLAYER_UPDATE_DATA: {
            PongProtocol::PlayerUpdateDataMessage playerUpdateDataMessage =
                PongProtocol::deserializePlayerUpdateData(signal);
            handlePlayerValues(playerUpdateDataMessage);
            break;
        }
        case PongProtocol::PLAYER_CONTROL: {
            _playerEntity = _entities[baseMessage.uuid];
            _registry.addComponent<RealEngine::Controllable>(_playerEntity,
                                                             RealEngine::Controllable{});
            break;
        }
        default:
            std::cout << "Unknown message type: " << baseMessage.messageType << std::endl;
            break;
    }
}

// for player and opponent
void pong::Game::handlePlayerMove(PongProtocol::PlayerMoveMessage parsedPacket) {
    const long int     uuid     = parsedPacket.uuid;
    const sf::Vector2f position = {parsedPacket.x, parsedPacket.y};
    float              step     = parsedPacket.step;

    auto it = _entities.find(uuid);
    if (it == _entities.end()) {
        return;
    }
    std::shared_ptr<RealEngine::Entity> entity = it->second;
    // auto* player_sprite          = _registry.getComponent<RealEngine::SpriteSheet>(entity);
    auto* positionComponent      = _registry.getComponent<RealEngine::Position>(entity);
    auto* interpolationComponent = _registry.getComponent<RealEngine::Interpolation>(entity);
    if (!positionComponent && !interpolationComponent) return;
    positionComponent->x                = interpolationComponent->end.x;
    positionComponent->y                = interpolationComponent->end.y;
    interpolationComponent->start       = {positionComponent->x, positionComponent->y};
    interpolationComponent->end         = position;
    interpolationComponent->step        = 1.f / step;
    interpolationComponent->currentStep = 0.f;
    interpolationComponent->reset       = true;
}

void pong::Game::handlePlayerValues(PongProtocol::PlayerUpdateDataMessage parsedPacket) {
    auto it = _entities.find(parsedPacket.uuid);
    if (it == _entities.end()) {
        return;
    }
    std::shared_ptr<RealEngine::Entity> entity = it->second;
    auto* scoreComponent                       = _registry.getComponent<RealEngine::Score>(entity);
    if (scoreComponent) {
        scoreComponent->amount = parsedPacket.score;
        std::cout << "Score: " << scoreComponent->amount << "for player: " << parsedPacket.uuid
                  << std::endl;
    }
}

void pong::Game::handleEntityUpdate(PongProtocol::EntityUpdateMessage parsedPacket) {
    auto it = _entities.find(parsedPacket.uuid);
    if (it == _entities.end()) return;
    std::shared_ptr<RealEngine::Entity> entity = it->second;
    auto* positionComponent      = _registry.getComponent<RealEngine::Position>(entity);
    auto* interpolationComponent = _registry.getComponent<RealEngine::Interpolation>(entity);
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

void pong::Game::handleNewEntity(PongProtocol::NewEntityMessage parsedPacket) {
    if (_entities.find(parsedPacket.uuid) != _entities.end()) {
        std::cout << "Entity with UUID " << parsedPacket.uuid << " already exists, skipping."
                  << std::endl;
        return;
    }
    std::cout << "New entity with UUID " << parsedPacket.uuid << std::endl;
    auto newEntity = _registry.spawnEntity();
    for (const auto& component : parsedPacket.components) {
        switch (component.first) {
            case PongProtocol::ComponentList::POSITION: {
                RealEngine::Position position;
                std::memcpy(&position, component.second.data(), sizeof(position));
                std::cout << "Position: (" << position.x << ", " << position.y << ")\n";
                _registry.addComponent(newEntity, RealEngine::Position{position.x, position.y});
                break;
            }
            case PongProtocol::ComponentList::INTERPOLATION: {
                RealEngine::Interpolation interpolation;
                std::memcpy(&interpolation, component.second.data(), sizeof(interpolation));
                _registry.addComponent(newEntity, RealEngine::Interpolation{interpolation});
                break;
            }
            case PongProtocol::ComponentList::VELOCITY: {
                RealEngine::Velocity velocity;
                std::memcpy(&velocity, component.second.data(), sizeof(velocity));
                std::cout << "Velocity: (" << velocity.vx << ", " << velocity.vy << ")\n";
                _registry.addComponent(
                    newEntity, RealEngine::Velocity{velocity.vx, velocity.vy, velocity.maxSpeed,
                                                    velocity.airFrictionForce});
                break;
            }
            case PongProtocol::ComponentList::SPRITE: {
                int sprite_id;
                std::memcpy(&sprite_id, component.second.data(), sizeof(sprite_id));
                std::string sprite_str = "";
                if (sprite_id == 1) {
                    sprite_str = "background";
                } else if (sprite_id == 2) {
                    sprite_str = "ball";
                } else if (sprite_id == 3) {
                    sprite_str = "player_bar";
                }
                std::cout << "Sprite: " << sprite_str << "\n";
                auto sprite = RealEngine::AssetManager::getInstance().getSprite(sprite_str);
                if (sprite) {
                    _registry.addComponent(*newEntity,
                                           RealEngine::SpriteComponent{*sprite, sprite_id});
                } else {
                    std::cerr << "Failed to load Sprite ID: " << sprite_str << std::endl;
                }
                break;
            }
            case PongProtocol::ComponentList::COLLISION: {
                // TODO: handle collision client side
                break;
            }
            case PongProtocol::ComponentList::DRAWABLE: {
                // std::cout << "Drawable: " << std::endl;
                _registry.addComponent<RealEngine::Drawable>(*newEntity, RealEngine::Drawable{});
                break;
            }
            case PongProtocol::ComponentList::ACCELERATION: {
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
    // std::cout << "New entity with UUID " << parsedPacket.uuid << " created." << std::endl;
    _registry.addComponent(newEntity, RealEngine::Netvar{"int", "uuid", parsedPacket.uuid});
    _entities.emplace(parsedPacket.uuid, newEntity);
}

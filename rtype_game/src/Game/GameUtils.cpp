/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** GameUtils
*/

#include "Game.hpp"

// void rtype::Game::handleSignal(std::string signal) {
//     if (signal.empty()) return;

//     // if signal contains "Event:Player_position" don't print it
//     if (signal.find("Event:Entity_position") == std::string::npos) {
//     }
//     std::unordered_map<std::string, std::string> parsedPacket =
//     PeterParser::parseMessage(signal);

//     if (parsedPacket.find("Event") != parsedPacket.end()) {
//         const std::string event = parsedPacket.at("Event");
//         if (event == "New_client") {
//             handleNewClient(parsedPacket);
//         }
//         if (event == "Synchronize") {
//             handleSynchronize(parsedPacket);
//         }
//         if (event == "Player_position") {
//             handlePlayerPosition(parsedPacket);
//         }
//         if (event == "New_entity") {
//             handleNewEntity(parsedPacket);
//         }
//         // if (event == "Entity_position") {
//         //     handleEntityPosition(parsedPacket);
//         // }
//         if (event == "Destroy_entity") {
//             handleDestroyEntity(parsedPacket);
//         }
//     }
// }

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
        case RTypeProtocol::SYNCHRONIZE: {
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
        case RTypeProtocol::EVENT_MESSAGE: {
            // Deserialize and handle event message
            RTypeProtocol::EventMessage eventMessage =
                RTypeProtocol::deserializeEventMessage(signal);
            // handleEventMessage(eventMessage);
            break;
        }
        default:
            // Handle unknown or unsupported message types (you can log or handle errors)
            std::cout << "Unknown message type: " << baseMessage.message_type << std::endl;
            break;
    }
}

// void rtype::Game::handleNewClient(std::unordered_map<std::string, std::string> parsedPacket) {
//     const sf::Vector2f position = PeterParser::parseVector2f(parsedPacket.at("Position"));
//     const long int     uuid     = std::stol(parsedPacket.at("Uuid"));
//     if (_players.find(uuid) != _players.end()) return;
//     add_player(uuid, position);
// }

void rtype::Game::handleNewClient(RTypeProtocol::PlayerMoveMessage parsedPacket) {
    const sf::Vector2f position = {parsedPacket.x, parsedPacket.y};
    const long int     uuid     = parsedPacket.uuid;
    if (_players.find(uuid) != _players.end()) return;
    add_player(uuid, position);
}

// void rtype::Game::handleSynchronize(std::unordered_map<std::string, std::string> parsedPacket) {
//     _localPlayerUUID                 = std::stol(parsedPacket.at("Uuid"));
//     const std::string players        = parsedPacket.at("Players");
//     _serverTime                      = std::stol(parsedPacket.at("Clock"));
//     auto client_now                  = std::chrono::steady_clock::now();
//     _startTime                       = client_now - std::chrono::milliseconds(_serverTime);
//     const std::string positions      = parsedPacket.at("Position");
//     sf::Vector2f      localPlayerPos = PeterParser::parseVector2f(positions);
//     std::cout << "Player uuid: " << _localPlayerUUID << std::endl;
//     _registry.add_component(_entity2, RealEngine::Position{localPlayerPos.x, localPlayerPos.y});
//     const std::vector<PeterParser::PlayerData> datas = PeterParser::parsePlayerList(players);
//     for (PeterParser::PlayerData player : datas) {
//         add_player(std::stol(player.uuid), player.position);
//     }
// }

void rtype::Game::handleSynchronize(RTypeProtocol::SynchronizeMessage parsedPacket) {
    _localPlayerUUID            = parsedPacket.uuid;
    _serverTime                 = parsedPacket.timestamp;
    auto client_now             = std::chrono::steady_clock::now();
    _startTime                  = client_now - std::chrono::milliseconds(_serverTime);
    sf::Vector2f localPlayerPos = {parsedPacket.x, parsedPacket.y};
    _registry.add_component(_entity2, RealEngine::Position{localPlayerPos.x, localPlayerPos.y});
    for (const auto& player : parsedPacket.players) {
        add_player(player.first, player.second);
    }
}

// void rtype::Game::handlePlayerPosition(std::unordered_map<std::string, std::string> parsedPacket)
// {
//     const long int     uuid     = std::stol(parsedPacket.at("Uuid"));
//     const sf::Vector2f position = PeterParser::parseVector2f(parsedPacket.at("Position"));
//     float              step     = std::stof(parsedPacket.at("Step"));
//     auto               it       = _players.find(uuid);

//     if (it == _players.end()) return;
//     std::shared_ptr<RealEngine::Entity> player = it->second;
//     // Update the position component of the player
//     auto* positionComponent      = _registry.get_component<RealEngine::Position>(player);
//     auto* interpolationComponent = _registry.get_component<RealEngine::Interpolation>(player);
//     if (!positionComponent && !interpolationComponent) return;
//     positionComponent->x                 = interpolationComponent->end.x;
//     positionComponent->y                 = interpolationComponent->end.y;
//     interpolationComponent->start        = {positionComponent->x, positionComponent->y};
//     interpolationComponent->end          = position;
//     interpolationComponent->step         = 1.f / step;
//     interpolationComponent->current_step = 0.f;
//     interpolationComponent->reset        = true;
// }

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

void rtype::Game::handleEntityPosition(std::unordered_map<std::string, std::string> parsedPacket) {
    const long int     uuid     = std::stol(parsedPacket.at("Uuid"));
    const sf::Vector2f position = PeterParser::parseVector2f(parsedPacket.at("Position"));
    // float              step     = std::stof(parsedPacket.at("Step"));
    auto it = _entities.find(uuid);

    if (it == _entities.end()) return;
    std::shared_ptr<RealEngine::Entity> entity = it->second;
    // Update the position component of the entity
    auto* positionComponent = _registry.get_component<RealEngine::Position>(entity);
    // auto* interpolationComponent = _registry.get_component<RealEngine::Interpolation>(entity);
    if (!positionComponent)
        // && !interpolationComponent)
        return;

    positionComponent->x = position.x;
    positionComponent->y = position.y;
    // positionComponent->x                 = interpolationComponent->end.x;
    // positionComponent->y                 = interpolationComponent->end.y;
    // interpolationComponent->start        = {positionComponent->x, positionComponent->y};
    // interpolationComponent->end          = position;
    // interpolationComponent->step         = 1.f / step;
    // interpolationComponent->current_step = 0.f;
    // interpolationComponent->reset        = true;
}

void rtype::Game::createPositionComponent(const std::string&                  value,
                                          std::shared_ptr<RealEngine::Entity> entity) {
    auto position = PeterParser::parseVector2f(value);
    _registry.add_component(entity, RealEngine::Position{position.x, position.y});
}

void rtype::Game::createVelocityComponent(const std::string&                  value,
                                          std::shared_ptr<RealEngine::Entity> entity) {
    std::vector<float> values = PeterParser::parseVelocity(value);
    // this parsing is ok for full args, will have problem with partial args TODO
    _registry.add_component(
        entity, RealEngine::Velocity{values[0], values[1], {values[2], values[3]}, values[4]});
}

void rtype::Game::createSpriteComponent(const std::string&                  value,
                                        std::shared_ptr<RealEngine::Entity> entity) {
    if (_textures.find(value) == _textures.end())
        std::cerr << "Texture not found for sprite: " << value << std::endl;
    auto sprite = RealEngine::Sprite{_textures[value]};
    sprite.setScale(GAME_SCALE, GAME_SCALE);
    _registry.add_component(entity, RealEngine::SpriteComponent{sprite});
}

void rtype::Game::createDrawableComponent(const std::string&                  value,
                                          std::shared_ptr<RealEngine::Entity> entity) {
    _registry.add_component(entity, RealEngine::Drawable{});
}

void rtype::Game::createAutoDestrcutibleComponent(const std::string&                  value,
                                                  std::shared_ptr<RealEngine::Entity> entity) {
    _registry.add_component(entity, RealEngine::AutoDestructible{std::stof(value)});
}

void rtype::Game::createRotationComponent(const std::string&                  value,
                                          std::shared_ptr<RealEngine::Entity> entity) {
    _registry.add_component(entity, RealEngine::Rotation{std::stof(value)});
}

// TODO: add create collision and type component functions

void rtype::Game::handleNewEntity(std::unordered_map<std::string, std::string> parsedPacket) {
    try {
        if (parsedPacket.find("Uuid") != parsedPacket.end()) {
            long int uuid = std::stol(parsedPacket.at("Uuid"));
            if (_entities.find(uuid) != _entities.end()) {
                return;
            }
            auto newEntity = _registry.spawn_entity();
            for (auto& [key, value] : parsedPacket) {
                if (_componentFunctions.find(key) != _componentFunctions.end()) {
                    _componentFunctions[key](value, newEntity);
                }
            }
            _entities.emplace(uuid, newEntity);
        } else {
            std::cerr << "Uuid not found in parsedPacket" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
    }
}

void rtype::Game::handleDestroyEntity(std::unordered_map<std::string, std::string> parsedPacket) {
    // std::unordered_map<long int, std::shared_ptr<RealEngine::Entity>> _entities;
    for (auto& [key, value] : parsedPacket) {
        if (key == "ids") {
            std::vector<long int> ids = PeterParser::parseIds(value);
            // delete entities with ids
            for (long int id : ids) {
                auto it = _entities.find(id);
                if (it != _entities.end()) {
                    _registry.remove_entity(*it->second);
                    _entities.erase(it);
                    continue;
                }
                // if not found try to find it inside the players
                auto playerIt = _players.find(id);
                if (playerIt != _players.end()) {
                    _registry.remove_entity(*playerIt->second);
                    _players.erase(playerIt);
                }
                if (_localPlayerUUID == id) {
                    _registry.remove_entity(*_entity2);
                    _entity2.reset();
                }
            }
            break;
        }
    }
}
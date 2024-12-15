/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** GameUtils
*/

#include "Game.hpp"

void rtype::Game::handleSignal(std::string signal) {
    if (signal.empty()) return;

    // if signal contains "Event:Player_position" don't print it
    if (signal.find("Event:Player_position") == std::string::npos)
        std::cout << "[" << signal << "]" << std::endl;
    std::unordered_map<std::string, std::string> parsedPacket = PeterParser::parseMessage(signal);

    if (parsedPacket.find("Event") != parsedPacket.end()) {
        const std::string event = parsedPacket.at("Event");
        if (event == "New_client") {
            handleNewClient(parsedPacket);
        }
        if (event == "Synchronize") {
            handleSynchronize(parsedPacket);
        }
        if (event == "Player_position") {
            handlePlayerPosition(parsedPacket);
        }
        if (event == "New_entity") {
            handleNewEntity(parsedPacket);
        }
    }
}

void rtype::Game::handleNewClient(std::unordered_map<std::string, std::string> parsedPacket) {
    const sf::Vector2f position = PeterParser::parseVector2f(parsedPacket.at("Position"));
    const long int     uuid     = std::stol(parsedPacket.at("Uuid"));
    add_player(uuid, position);
}

void rtype::Game::handleSynchronize(std::unordered_map<std::string, std::string> parsedPacket) {
    _localPlayerUUID                 = std::stol(parsedPacket.at("Uuid"));
    const std::string players        = parsedPacket.at("Players");
    _serverTime                      = std::stol(parsedPacket.at("Clock"));
    auto client_now                  = std::chrono::steady_clock::now();
    _startTime                       = client_now - std::chrono::milliseconds(_serverTime);
    const std::string positions      = parsedPacket.at("Position");
    sf::Vector2f      localPlayerPos = PeterParser::parseVector2f(positions);
    _registry.add_component(_entity2, RealEngine::Position{localPlayerPos.x, localPlayerPos.y});
    const std::vector<PeterParser::PlayerData> datas = PeterParser::parsePlayerList(players);
    for (PeterParser::PlayerData player : datas) {
        add_player(std::stol(player.uuid), player.position);
    }
}

void rtype::Game::handlePlayerPosition(std::unordered_map<std::string, std::string> parsedPacket) {
    const long int     uuid     = std::stol(parsedPacket.at("Uuid"));
    const sf::Vector2f position = PeterParser::parseVector2f(parsedPacket.at("Position"));
    float              step     = std::stof(parsedPacket.at("Step"));
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
    auto sprite = RealEngine::Sprite{value};
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

// TODO: add create collision and type component functions

void rtype::Game::handleNewEntity(std::unordered_map<std::string, std::string> parsedPacket) {
    auto newEntity = _registry.spawn_entity();
    std::cout << "New entity: " << *newEntity << std::endl;
    for (auto& [key, value] : parsedPacket) {
        std::cout << key << " : " << value << std::endl;
        if (_componentFunctions.find(key) != _componentFunctions.end()) {
            _componentFunctions[key](value, newEntity);
        } else {
            std::cout << "No component found for key: " << key << std::endl;
        }
    }
}
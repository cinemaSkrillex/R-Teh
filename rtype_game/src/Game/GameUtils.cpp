/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** GameUtils
*/

#include "Game.hpp"

void rtype::Game::handleSignal(std::string signal) {
    if (signal.empty()) return;

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
    float        step     = std::stof(parsedPacket.at("Step"));
    auto               it       = _players.find(uuid);

    if (it == _players.end()) return;
    RealEngine::Entity player = it->second;
    // Update the position component of the player
    auto* positionComponent      = _registry.get_component<RealEngine::Position>(player);
    auto* interpolationComponent = _registry.get_component<RealEngine::Interpolation>(player);
    if (positionComponent && interpolationComponent && interpolationComponent->current_step >= 1.f) {
        interpolationComponent->start        = {positionComponent->x, positionComponent->y};
        interpolationComponent->end          = position;
        interpolationComponent->step         = 1.f / step;
        interpolationComponent->current_step = 0.f;
    }
}
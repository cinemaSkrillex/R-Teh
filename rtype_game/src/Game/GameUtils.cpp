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
    std::unordered_map<std::string, std::string> parsedPacket = parseMessage(signal);

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
    const sf::Vector2f position = parsePosition(parsedPacket.at("Position"));
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
    sf::Vector2f      localPlayerPos = parsePosition(positions);
    _registry.add_component(_entity2, RealEngine::Position{localPlayerPos.x, localPlayerPos.y});
    const std::vector<PlayerData> datas = parsePlayerList(players);
    for (PlayerData player : datas) {
        add_player(std::stol(player.uuid), player.position);
    }
}

void rtype::Game::handlePlayerPosition(std::unordered_map<std::string, std::string> parsedPacket) {
    const long int     uuid     = std::stol(parsedPacket.at("Uuid"));
    const sf::Vector2f position = parsePosition(parsedPacket.at("Position"));

    // Find the player entity by UUID
    auto it = _players.find(uuid);
    if (it != _players.end()) {
        RealEngine::Entity player = it->second;
        // Update the position component of the player
        auto* positionComponent = _registry.get_component<RealEngine::Position>(player);
        if (positionComponent) {
            positionComponent->x = position.x;
            positionComponent->y = position.y;
        } else {
            // If the position component does not exist, add it
            _registry.add_component(player, RealEngine::Position{position.x, position.y});
        }
    }
}
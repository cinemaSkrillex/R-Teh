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
            const sf::Vector2f position = parsePosition(parsedPacket.at("Position"));
            const long int     uuid     = std::stol(parsedPacket.at("Uuid"));
        } else if (event == "Synchronize") {
            _localPlayerUUID                 = std::stol(parsedPacket.at("Uuid"));
            const std::string players        = parsedPacket.at("Players");
            _serverTime                      = std::stol(parsedPacket.at("Clock"));
            auto client_now                  = std::chrono::steady_clock::now();
            _startTime                       = client_now - std::chrono::milliseconds(_serverTime);
            const std::string positions      = parsedPacket.at("Position");
            sf::Vector2f      localPlayerPos = parsePosition(positions);
            _registry.add_component(_entity2,
                                    RealEngine::Position{localPlayerPos.x, localPlayerPos.y});
            const std::vector<PlayerData> datas = parsePlayerList(players);
            for (PlayerData player : datas) {
                add_player(std::stol(player.uuid), player.position);
            }
        } else if (event == "Player_position") {
            const long int     uuid     = std::stol(parsedPacket.at("Uuid"));
            const sf::Vector2f position = parsePosition(parsedPacket.at("Position"));
            _registry.add_component<RealEngine::Position>(_entity2, {position.x, position.y});
        }
    }
}
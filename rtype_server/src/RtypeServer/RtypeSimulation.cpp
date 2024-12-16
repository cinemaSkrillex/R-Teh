/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeSimulation
*/

#include "../../include/RtypeServer.hpp"

void RtypeServer::runSimulation(const std::unordered_map<std::string, std::string>& parsed_data,
                                asio::ip::udp::endpoint& client, Player& player) {
    auto       player_direction_x  = PeterParser::parseVector2f(parsed_data.at("DirectionX"));
    auto       player_direction_y  = PeterParser::parseVector2f(parsed_data.at("DirectionY"));
    const auto player_uuid         = std::stol(parsed_data.at("Uuid"));
    const auto timestamp           = std::stol(parsed_data.at("Timestamp"));
    const auto lastTimestamp       = _players.at(client).getLastTimestamp();
    long       client_elapsed_time = timestamp - lastTimestamp;
    float      client_elapsed_time_seconds = client_elapsed_time / 1000.f;

    _players.at(client).setLastTimestamp(timestamp);

    // Use consistent server delta time for simulation
    _game_instance->movePlayer(
        player_uuid,
        sf::IntRect(static_cast<int>(player_direction_x.x), static_cast<int>(player_direction_x.y),
                    static_cast<int>(player_direction_y.x), static_cast<int>(player_direction_y.y)),
        client_elapsed_time_seconds);
    _game_instance->runPlayerSimulation(_players.at(client).getEntity(),
                                        client_elapsed_time_seconds);
}
/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeSimulation
*/

#include "../../include/RtypeServer.hpp"
#include "Log.hpp"

void RtypeServer::runSimulation(const std::unordered_map<std::string, std::string>& parsed_data,
                                asio::ip::udp::endpoint& client, Player& player) {
    const auto player_direction    = PeterParser::parseVector2f(parsed_data.at("Direction"));
    const auto player_uuid         = std::stol(parsed_data.at("Uuid"));
    const auto timestamp           = std::stol(parsed_data.at("Timestamp"));
    const auto lastTimestamp       = _players.at(client).getLastTimestamp();
    long       client_elapsed_time = timestamp - lastTimestamp;
    float      client_elapsed_time_seconds = client_elapsed_time / 1000.f;

    _players.at(client).setLastTimestamp(timestamp);

    // Use consistent server delta time for simulation
    _game_instance->movePlayer(player_uuid, player_direction, client_elapsed_time_seconds);
    _game_instance->runPlayerSimulation(*_players.at(client).getEntity(), client_elapsed_time_seconds);
}
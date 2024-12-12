/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerRun
*/

#include "../include/RtypeServer.hpp"
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
    _game_instance->run(*_players.at(client).getEntity(), client_elapsed_time_seconds);
}

void RtypeServer::run() {
    auto log = std::make_shared<Log>("RtypeServer.log");

    while (true) {
        if (_clock.getElapsedTime().asMilliseconds() > 1000 / SERVER_TICK) {
            // Reset the clock for the next tick
            _deltaTime = _clock.restart().asSeconds();

            // Do server work
            for (auto client : _server->getClients()) {
                // Process all messages from the client
                for (const auto& message : _server->get_unreliable_messages_from_endpoint(client)) {
                    const auto parsed_data = PeterParser::parseMessage(message);

                    if (parsed_data.find("Event") != parsed_data.end()) {
                    } else {
                        runSimulation(parsed_data, client, _players.at(client));
                    }
                }
            }

            if (_broadcastClock.getElapsedTime().asMilliseconds() > 1000 / SERVER_BROADCAST_TICK) {
                _deltaTimeBroadcast = _broadcastClock.restart().asSeconds();
                for (const auto& player : _players) {
                    broadcastPlayerState(player.second);
                }
            }
        }
    }
}
/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerRun
*/

#include "../include/RtypeServer.hpp"

void RtypeServer::run() {
    while (true) {
        if (_clock.getElapsedTime().asMilliseconds() > 1000 / SERVER_TICK) {
            // Reset the clock for the next tick
            _deltaTime = _clock.restart().asSeconds();

            // Do server work
            for (auto client : _server->getClients()) {
                // Process all messages from the client
                for (const auto& message : _server->get_unreliable_messages_from_endpoint(client)) {
                    const auto parsed_data         = parseMessage(message);
                    const auto player_direction    = parseDirection(parsed_data.at("Direction"));
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
            }
        }
    }
}
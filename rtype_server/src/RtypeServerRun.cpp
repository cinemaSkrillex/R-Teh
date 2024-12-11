/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerRun
*/

#include "../include/RtypeServer.hpp"
#include "Log.hpp"

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
                    const auto parsed_data         = parseMessage(message);
                    const auto player_direction    = parseDirection(parsed_data.at("Direction"));
                    const auto player_uuid         = std::stol(parsed_data.at("Uuid"));
                    const auto timestamp           = std::stol(parsed_data.at("Timestamp"));
                    const auto lastTimestamp       = _players.at(client).getLastTimestamp();
                    long       client_elapsed_time = timestamp - lastTimestamp;
                    float      client_elapsed_time_seconds = client_elapsed_time / 1000.f;

                    _players.at(client).setLastTimestamp(timestamp);

                    // Use consistent server delta time for simulation
                    _game_instance->movePlayer(player_uuid, player_direction,
                                               client_elapsed_time_seconds);
                    _game_instance->run(*_players.at(client).getEntity(),
                                        client_elapsed_time_seconds);
                }
            }
        }
        if (_broadcastClock.getElapsedTime().asMilliseconds() > 1000 / SERVER_BROADCAST_TICK) {
            _broadcastClock.restart();
            for (const auto& player : _players) {
                RealEngine::Entity* entity = player.second.getEntity();
                auto*               position =
                    _game_instance->getRegistryRef().get_component<RealEngine::Position>(*entity);
                if (position) {
                    std::string message =
                        "Event:Player_position Uuid:" + std::to_string(player.second.getUUID()) +
                        " Position:(" + std::to_string(position->x) + "," +
                        std::to_string(position->y) + ")";
                    for (auto client : _server->getClients()) {
                        _server->send_unreliable_packet(message, client);
                    }
                }
            }
        }
    }
}
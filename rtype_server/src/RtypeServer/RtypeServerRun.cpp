/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerRun
*/

#include "../../include/RtypeServer.hpp"

void RtypeServer::run() {
    auto log = std::make_shared<Log>("RtypeServer.log");

    while (true) {
        int server_tick = _server_config.getConfigItem<int>("SERVER_TICK");
        if (_clock.getElapsedTime().asMilliseconds() > 1000 / server_tick) {
            // Reset the clock for the next tick
            _deltaTime = _clock.restart().asSeconds();

            // Do server work
            for (auto client : _server->getClients()) {
                // Process all messages from the client
                for (const auto& message : _server->get_unreliable_messages_from_endpoint(client)) {
                    const auto parsed_data = PeterParser::parseMessage(message);
                    // std::cout << "Received message: " << message << std::endl;

                    if (parsed_data.find("Event") != parsed_data.end()) {
                        runEvent(parsed_data, client, _players.at(client));
                    } else {
                        runSimulation(parsed_data, client, _players.at(client));
                    }
                }
            }
            _game_instance->run(_deltaTime);
        }
        int server_broadcast_tick = _server_config.getConfigItem<int>("SERVER_BROADCAST_TICK");
        if (_broadcastClock.getElapsedTime().asMilliseconds() > 1000 / server_broadcast_tick) {
            _deltaTimeBroadcast = _broadcastClock.restart().asSeconds();
            for (const auto& player : _players) {
                broadcastPlayerState(player.second);
                for (const auto& mob : _game_instance->getSimpleMobs()) {
                    broadcastEntityState(*mob, mob);
                }
            }
        }
    }
}
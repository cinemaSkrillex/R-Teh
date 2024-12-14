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
        if (_clock.getElapsedTime().asMilliseconds() > 1000 / _config.SERVER_TICK) {
            // Reset the clock for the next tick
            _deltaTime = _clock.restart().asSeconds();

            // Do server work
            for (auto client : _server->getClients()) {
                // Process all messages from the client
                for (const auto& message : _server->get_unreliable_messages_from_endpoint(client)) {
                    const auto parsed_data = PeterParser::parseMessage(message);
                    std::cout << "Received message: " << message << std::endl;

                    if (parsed_data.find("Event") != parsed_data.end()) {
                        runEvent(parsed_data, client, _players.at(client));
                    } else {
                        runSimulation(parsed_data, client, _players.at(client));
                    }
                }
            }

            _game_instance->run(_deltaTime);
        }
        if (_broadcastClock.getElapsedTime().asMilliseconds() >
            1000 / _config.SERVER_BROADCAST_TICK) {
            _deltaTimeBroadcast = _broadcastClock.restart().asSeconds();
            for (const auto& player : _players) {
                broadcastPlayerState(player.second);
                // for (const auto entity : _server_entities) {
                //     broadcastEntityState(entity.first, entity.second);
                // }
            }
        }
    }
}
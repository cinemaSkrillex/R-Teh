/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerRun
*/

#include "../../include/RtypeServer.hpp"

void RtypeServer::run() {
    auto log                   = std::make_shared<Log>("RtypeServer.log");
    int  server_tick           = _server_config.getConfigItem<int>("SERVER_TICK");
    int  server_broadcast_tick = _server_config.getConfigItem<int>("SERVER_BROADCAST_TICK");

    while (true) {
        if (_clock.getElapsedTime().asMilliseconds() > 1000 / server_tick) {
            // Reset the clock for the next tick
            _deltaTime = _clock.restart().asSeconds();

            // Do server work
            for (auto client : _server->getClients()) {
                // Process all messages from the client
                for (const auto& message : _server->get_unreliable_messages_from_endpoint(client)) {
                    const auto parsed_data = PeterParser::parseMessage(message);

                    if (parsed_data.find("Event") != parsed_data.end()) {
                        runEvent(parsed_data, client, _players.at(client));
                    } else {
                        runSimulation(parsed_data, client, _players.at(client));
                    }
                }
            }
            auto destroyedEntities = _game_instance->run(_deltaTime);
            if (!destroyedEntities.empty()) {
                std::string message = "Event:Destroy_entity ids:[";
                for (auto entity : destroyedEntities) {
                    message += std::to_string(entity) + ",";
                }
                message.pop_back();
                message += "]";
                broadCastAll(message);
            }
        }
        if (_broadcastClock.getElapsedTime().asMilliseconds() > 1000 / server_broadcast_tick) {
            _deltaTimeBroadcast = _broadcastClock.restart().asSeconds();
            for (const auto& player : _players) {
                broadcastPlayerState(player.second);
                // for (const auto& mob : _game_instance->getSimpleMobs()) {
                //     broadcastEntityState(*mob, mob);
                // }
            }
        }
    }
}
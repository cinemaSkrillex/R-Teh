/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerRun
*/

#include "../../include/RtypeServer/RtypeServer.hpp"
#include "../../include/shared/RtypeServerProtocol.hpp"

void RtypeServer::run() {
    auto log                   = std::make_shared<Log>("RtypeServer.log");
    int  server_tick           = _server_config.getConfigItem<int>("SERVER_TICK");
    int  server_broadcast_tick = _server_config.getConfigItem<int>("SERVER_BROADCAST_TICK");

    while (true) {
        if (_clock.getElapsedTime().asMilliseconds() > 1000 / server_tick) {
            // Reset the clock for the next tick
            _deltaTime = _clock.restart().asSeconds();

            // Do server work
            for (const auto& client : _server->getClients()) {  // Use reference to avoid copying
                auto& player = _players.at(client);  // Store reference to avoid repeated lookups
                const auto& messages = _server->get_unreliable_messages_from_endpoint(client);

                for (const auto& message : messages) {
                    auto baseMessage = RTypeProtocol::deserialize<800>(message);

                    if (baseMessage.message_type == RTypeProtocol::PLAYER_DIRECTION) {
                        runSimulation(message, client, player);
                    } else {
                        runEvent(message, client, player);
                    }
                }
            }
            auto destroyedEntities = _game_instance->run(_deltaTime);
            if (!destroyedEntities.empty()) {
                RTypeProtocol::DestroyEntityMessage destroyMessage;
                destroyMessage.message_type = RTypeProtocol::MessageType::DESTROY_ENTITY;
                destroyMessage.uuid         = 0;
                destroyMessage.entity_ids.reserve(destroyedEntities.size());
                destroyMessage.entity_ids.insert(destroyMessage.entity_ids.end(),
                                                 destroyedEntities.begin(),
                                                 destroyedEntities.end());
                std::array<char, 800> serializedDestroyMessage =
                    RTypeProtocol::serialize<800>(destroyMessage);
                broadcastAllReliable(serializedDestroyMessage);
            }
        }
        if (_broadcastClock.getElapsedTime().asMilliseconds() > 1000 / server_broadcast_tick) {
            _deltaTimeBroadcast = _broadcastClock.restart().asSeconds();
            for (const auto& player : _players) {
                broadcastPlayerState(player.second);
            }
        }
    }
}
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
            for (auto client : _server->getClients()) {
                // Process all messages from the client
                for (const auto& message : _server->get_unreliable_messages_from_endpoint(client)) {
                    RTypeProtocol::BaseMessage baseMessage =
                        RTypeProtocol::deserialize<800>(message);

                    if (baseMessage.message_type == RTypeProtocol::PLAYER_DIRECTION) {
                        runSimulation(message, client, _players.at(client));
                    } else {
                        runEvent(message, client, _players.at(client));
                    }
                }
            }
            auto destroyedEntities = _game_instance->run(_deltaTime);
            if (!destroyedEntities.empty()) {
                RTypeProtocol::DestroyEntityMessage destroyMessage;
                destroyMessage.message_type = RTypeProtocol::MessageType::DESTROY_ENTITY;
                destroyMessage.uuid         = 0;
                for (auto entity : destroyedEntities) {
                    destroyMessage.entity_ids.push_back(entity);
                }
                std::array<char, 800> serializedDestroyMessage =
                    RTypeProtocol::serialize<800>(destroyMessage);
                broadCastAll(serializedDestroyMessage);
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
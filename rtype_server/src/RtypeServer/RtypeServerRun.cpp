/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerRun
*/

#include "../../include/RtypeServer.hpp"
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
                    // const auto parsed_data = PeterParser::parseMessage(message);
                    RTypeProtocol::BaseMessage baseMessage =
                        RTypeProtocol::deserialize<800>(message);

                    if (baseMessage.message_type == RTypeProtocol::PLAYER_DIRECTION) {
                        runSimulation(message, client, _players.at(client));
                    } else if (baseMessage.message_type == RTypeProtocol::EVENT_MESSAGE) {
                        runEvent(message, client, _players.at(client));
                    } else {
                        // Handle unknown or unsupported message types (you can log or handle
                        // errors)
                        std::cout << "Unknown message type: " << baseMessage.message_type
                                  << std::endl;
                        break;
                    }

                    // event is only used for shooting right now so we will just run a
                    // simulation for now. if (parsed_data.find("Event") != parsed_data.end()) {
                    // runEvent(parsed_data, client, _players.at(client));
                    // } else {
                    // runSimulation(parsed_data, client, _players.at(client));
                    // }
                }
            }
            auto destroyedEntities = _game_instance->run(_deltaTime);
            if (!destroyedEntities.empty()) {
                // std::string message = "Event:Destroy_entity ids:[";
                // for (auto entity : destroyedEntities) {
                //     message += std::to_string(entity) + ",";
                // }
                // message.pop_back();
                // message += "]";
                // std::cout << message << std::endl;
                // broadCastAll(message);
                RTypeProtocol::DestroyEntityMessage destroyMessage;
                destroyMessage.message_type = RTypeProtocol::MessageType::DESTROY_ENTITY;
                destroyMessage.uuid         = 0;
                for (auto entity : destroyedEntities) {
                    destroyMessage.entity_ids.push_back(entity);
                }
                // auto serializedMessage = RTypeProtocol::serialize(destroyMessage);
                std::array<char, 800> serializedDestroyMessage =
                    RTypeProtocol::serialize<800>(destroyMessage);
                broadCastAll(serializedDestroyMessage);
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
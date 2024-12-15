/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerCallbacks
*/

#include "../../include/RtypeServer.hpp"

void RtypeServer::initCallbacks() {
    _server->setNewClientCallback([this](const asio::ip::udp::endpoint& sender) {
        std::cout << "Callback: New client connected from " << sender.address() << std::endl;

        UUIDGenerator uuid_generator;
        long int      uuid = uuid_generator.generate_long();
        std::cout << "Generated UUID: " << uuid << std::endl;
        // Notify all other clients about the new client
        for (const auto& client : _server->getClients()) {
            if (client != sender) {
                const std::string message = "Event:New_client Uuid:" + std::to_string(uuid) +
                                            " Position:(" +
                                            std::to_string(PLAYER_START_POSITION.x) + "," +
                                            std::to_string(PLAYER_START_POSITION.y) + ")";
                _server->send_reliable_packet(message, client);
            }
        }
        // Create the uuid for each new client
        std::string message = "Event:Synchronize Uuid:" + std::to_string(uuid) +
                              " Clock:" + formatTimestamp(_startTime) + " Position:(" +
                              std::to_string(PLAYER_START_POSITION.x) + "," +
                              std::to_string(PLAYER_START_POSITION.y) + ") Players:[";
        bool first = true;
        for (const auto& player_pair : _players) {
            if (!first) message += "|";
            first              = false;
            const auto& player = player_pair.second;
            message += std::to_string(player.getUUID()) + ",(" +
                       std::to_string(player.getPosition().x) + "," +
                       std::to_string(player.getPosition().y) + ")";
        }
        message += "]";
        _server->send_reliable_packet(message, sender);

        // Send all the entities to the new client, so it can synchronize and move
        for (const auto& mob : _game_instance->getSimpleMobs()) {
            if (!mob) continue;
            auto* position =
                _game_instance->getRegistry()->get_component<RealEngine::Position>(mob);
            if (!position) continue;
            auto *destructible = _game_instance->getRegistry()->get_component<RealEngine::AutoDestructible>(mob);
            std::string MobMessage = "Event:New_entity";
            MobMessage += " Type:mob";
            MobMessage += " Sprite:../../assets/sprites/the_eye/bomber.png";
            MobMessage += " Position:(" + std::to_string(position->x) + "," +
                          std::to_string(position->y);
            MobMessage += " Velocity:(-250,0,{1000,500},0)";
            MobMessage += " Collision:(0,0,16,8,mob,false,OTHER)";
            MobMessage += " AutoDestructible:" + std::to_string(destructible->lifeTime);
            MobMessage += " Drawable:true";
            _server->send_reliable_packet(MobMessage, sender);
        }

        long elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::steady_clock::now() - _startTime)
                                .count();
        auto player =
            Player(uuid, elapsed_time, _game_instance->addAndGetPlayer(uuid, PLAYER_START_POSITION),
                   _game_instance->getRegistry());
        _players[sender] = player;
    });
}
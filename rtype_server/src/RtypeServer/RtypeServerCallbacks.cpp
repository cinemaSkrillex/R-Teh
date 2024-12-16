/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerCallbacks
*/

#include "../../include/RtypeServer.hpp"

void RtypeServer::initCallbacks() {
    _server->setNewClientCallback([this](const asio::ip::udp::endpoint& sender) {
        sf::Vector2f player_start_position =
            _server_config.getConfigItem<sf::Vector2f>("PLAYER_START_POSITION");
        // create Player entity
        long elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::steady_clock::now() - _startTime)
                                .count();
        auto playerEntity = _game_instance->addAndGetPlayer(player_start_position);
        auto player =
            Player(*playerEntity, elapsed_time, playerEntity, _game_instance->getRegistry());
        // Notify all other clients about the new client
        for (const auto& client : _server->getClients()) {
            if (client != sender) {
                const std::string message =
                    "Event:New_client Uuid:" + std::to_string(*playerEntity) + " Position:(" +
                    std::to_string(player_start_position.x) + "," +
                    std::to_string(player_start_position.y) + ")";
                _server->send_reliable_packet(message, client);
            }
        }
        // Create the uuid for each new client
        std::string message = "Event:Synchronize Uuid:" + std::to_string(*playerEntity) +
                              " Clock:" + formatTimestamp(_startTime) + " Position:(" +
                              std::to_string(player_start_position.x) + "," +
                              std::to_string(player_start_position.y) + ") Players:[";
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
            auto* destructible =
                _game_instance->getRegistry()->get_component<RealEngine::AutoDestructible>(mob);
            if (!destructible) continue;
            auto* velocity =
                _game_instance->getRegistry()->get_component<RealEngine::Velocity>(mob);
            if (!velocity) continue;
            auto* rotation =
                _game_instance->getRegistry()->get_component<RealEngine::Rotation>(mob);

            std::string velocityStr =
                std::to_string(velocity->vx) + "," + std::to_string(velocity->vy) + ",{" +
                std::to_string(velocity->maxSpeed.x) + "," + std::to_string(velocity->maxSpeed.y) +
                "}," + std::to_string(velocity->airFrictionForce);
            std::string MobMessage = "Event:New_entity";
            MobMessage += " Type:mob";
            MobMessage += " Uuid:" + std::to_string(*mob);
            MobMessage += " Sprite:enemy";
            MobMessage += " Position:(" + std::to_string(position->x) + "," +
                          std::to_string(position->y) + ")";
            MobMessage += " Velocity:(" + velocityStr + ")";
            MobMessage += " Collision:(0,0,16,8,mob,false,OTHER)";
            MobMessage += " AutoDestructible:" + std::to_string(destructible->lifeTime);
            if (rotation) MobMessage += " Rotation:" + std::to_string(rotation->angle);
            MobMessage += " Drawable:true";

            _server->send_reliable_packet(MobMessage, sender);
        }
        _players[sender] = player;
    });
}
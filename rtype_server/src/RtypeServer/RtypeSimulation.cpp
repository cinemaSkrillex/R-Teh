/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeSimulation
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

void RtypeServer::runSimulation(const std::array<char, 800>&   buffer,
                                const asio::ip::udp::endpoint& client, ServerPlayer& player) {
    // Deserialize the PlayerDirectionMessage
    RTypeProtocol::PlayerDirectionMessage playerDirectionMessage =
        RTypeProtocol::deserializePlayerDirection<800>(buffer);
    const auto player_uuid                 = playerDirectionMessage.uuid;
    const auto timestamp                   = playerDirectionMessage.timestamp;
    const auto lastTimestamp               = _players.at(client).getLastTimestamp();
    long       client_elapsed_time         = timestamp - lastTimestamp;
    float      client_elapsed_time_seconds = static_cast<float>(client_elapsed_time) / 1000.f;

    _players.at(client).setLastTimestamp(timestamp);

    // Use consistent server delta time for simulation
    _gameInstance->movePlayer(player_uuid, playerDirectionMessage.direction,
                              client_elapsed_time_seconds);
    _gameInstance->runPlayerSimulation(_players.at(client).getEntity(),
                                       client_elapsed_time_seconds);
}
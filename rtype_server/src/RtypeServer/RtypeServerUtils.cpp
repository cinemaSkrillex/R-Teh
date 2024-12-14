/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerUtils
*/

#include "../../include/RtypeServer.hpp"

std::string RtypeServer::formatTimestamp(const std::chrono::steady_clock::time_point& start_time) {
    auto now     = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
    return std::to_string(elapsed);  // in milliseconds
}

void RtypeServer::broadcastPlayerState(const Player& player) {
    // Get the player's position
    std::shared_ptr<RealEngine::Entity> entity = player.getEntity();
    auto* position = _game_instance->getRegistryRef().get_component<RealEngine::Position>(*entity);
    if (position) {
        std::string message = "Event:Player_position Uuid:" + std::to_string(player.getUUID()) +
                              " Step:" + std::to_string(_deltaTimeBroadcast) + " Position:(" +
                              std::to_string(position->x) + "," + std::to_string(position->y) + ")";
        for (auto client : _server->getClients()) {
            _server->send_unreliable_packet(message, client);
        }
    }
}
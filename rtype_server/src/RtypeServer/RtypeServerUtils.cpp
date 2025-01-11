/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** RtypeServerUtils
*/

#include "../../include/RtypeServer/RtypeServer.hpp"

std::string RtypeServer::formatTimestamp(const std::chrono::steady_clock::time_point& start_time) {
    auto now     = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
    return std::to_string(elapsed);  // in milliseconds
}

void RtypeServer::broadcastPlayerState(const Player& player) {
    // Get the player's position
    std::shared_ptr<RealEngine::Entity> entity = player.getEntity();
    if (!entity) {
        return;
    }
    auto& registry = _game_instance->getRegistryRef();
    auto* position = registry.get_component<RealEngine::Position>(*entity);
    if (!position) {
        return;
    }

    // Create a PlayerMoveMessage
    RTypeProtocol::PlayerMoveMessage playerMoveMessage = {};
    playerMoveMessage.message_type                     = RTypeProtocol::PLAYER_MOVE;
    playerMoveMessage.uuid                             = player.getUUID();
    playerMoveMessage.x                                = position->x;
    playerMoveMessage.y                                = position->y;
    playerMoveMessage.step                             = _deltaTimeBroadcast;
    playerMoveMessage.timestamp = std::chrono::system_clock::now().time_since_epoch().count();

    // Serialize the PlayerMoveMessage
    std::array<char, 800> serializedMessage = RTypeProtocol::serialize<800>(playerMoveMessage);

    // Broadcast the serialized message to all clients
    broadcastAllUnreliable(serializedMessage);
}

void RtypeServer::broadcastEntityState(int uuid, const std::shared_ptr<RealEngine::Entity> entity) {
    auto* position = _game_instance->getRegistryRef().get_component<RealEngine::Position>(*entity);
    if (position) {
        std::string message = "Event:Entity_position Uuid:" + std::to_string(uuid) +
                              " Step:" + std::to_string(_deltaTimeBroadcast) + " Position:(" +
                              std::to_string(position->x) + "," + std::to_string(position->y) + ")";
        for (auto client : _server->getClients()) {
            // _server->send_unreliable_packet(message, client);
        }
    }
}

void RtypeServer::broadcastAllReliable(const std::array<char, 800>& message) {
    for (const auto& client : _server->getClients()) {
        _server->send_reliable_packet(message, client);
    }
}

void RtypeServer::broadcastAllUnreliable(const std::array<char, 800>& message) {
    for (const auto& client : _server->getClients()) {
        _server->send_unreliable_packet(message, client);
    }
}
/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** PongControls.cpp
*/

#include "../include/Game/PongControls.hpp"

namespace pong {

Controls::Controls(RealEngine::Registry& registry, std::shared_ptr<UDPClient> client)
    : _registry(registry), _client(client) {}

Controls::~Controls() {}

void Controls::moveUp(float deltaTime, RealEngine::Entity entity) {
    auto* velocity     = _registry.get_component<RealEngine::Velocity>(entity);
    auto* acceleration = _registry.get_component<RealEngine::Acceleration>(entity);
    if (velocity->vy > 50) velocity->vy = 50;
    velocity->vy -= acceleration->ay * 3 * deltaTime;
}

void Controls::moveDown(float deltaTime, RealEngine::Entity entity) {
    auto* velocity     = _registry.get_component<RealEngine::Velocity>(entity);
    auto* acceleration = _registry.get_component<RealEngine::Acceleration>(entity);
    if (velocity->vy < -50) velocity->vy = -50;
    velocity->vy += acceleration->ay * 3 * deltaTime;
}

void Controls::sendReady(float deltaTime, RealEngine::Entity entity) {
    PongProtocol::BaseMessage eventMessage;
    eventMessage.message_type = PongProtocol::MessageType::PLAYER_READY;

    std::array<char, 800> serializedEventMessage = PongProtocol::serialize<800>(eventMessage);
    _client->send_unreliable_packet(serializedEventMessage);
}

}  // namespace pong
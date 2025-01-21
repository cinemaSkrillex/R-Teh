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
    auto* velocity     = _registry.getComponent<RealEngine::Velocity>(entity);
    auto* acceleration = _registry.getComponent<RealEngine::Acceleration>(entity);
    if (velocity->vy > 50) velocity->vy = 50;
    velocity->vy -= acceleration->ay * 3 * deltaTime;
}

void Controls::moveDown(float deltaTime, RealEngine::Entity entity) {
    auto* velocity     = _registry.getComponent<RealEngine::Velocity>(entity);
    auto* acceleration = _registry.getComponent<RealEngine::Acceleration>(entity);
    if (velocity->vy < -50) velocity->vy = -50;
    velocity->vy += acceleration->ay * 3 * deltaTime;
}

void Controls::sendReady(float deltaTime, RealEngine::Entity entity) {
    PongProtocol::BaseMessage eventMessage;
    eventMessage.messageType = PongProtocol::MessageType::PLAYER_READY;
    auto* playerNetvar       = _registry.getComponent<RealEngine::Netvar>(entity);
    if (!playerNetvar) {
        std::cout << "Player does not have netvar component" << std::endl;
    } else {
        eventMessage.uuid = std::any_cast<long int>(playerNetvar->value);
    }

    std::array<char, 800> serializedEventMessage = PongProtocol::serialize<800>(eventMessage);
    _client->sendUnreliablePacket(serializedEventMessage);
}

}  // namespace pong
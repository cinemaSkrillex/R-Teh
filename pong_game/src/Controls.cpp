/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Controls
*/

#include "Controls.hpp"

namespace rtype {

Controls::Controls(RealEngine::Registry& registry, std::shared_ptr<UDPClient> client)
    : _registry(registry), _client(client), _particlesTimer(0.1f) {}

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

void Controls::moveLeft(float deltaTime, RealEngine::Entity entity) {
    auto* velocity     = _registry.get_component<RealEngine::Velocity>(entity);
    auto* acceleration = _registry.get_component<RealEngine::Acceleration>(entity);
    if (velocity->vx > 50) velocity->vx = 50;
    velocity->vx -= acceleration->ax * 3 * deltaTime;
}

void Controls::moveRight(float deltaTime, RealEngine::Entity entity) {
    auto* velocity     = _registry.get_component<RealEngine::Velocity>(entity);
    auto* acceleration = _registry.get_component<RealEngine::Acceleration>(entity);
    if (velocity->vx < -50) velocity->vx = -50;
    velocity->vx += acceleration->ax * 3 * deltaTime;
}

void Controls::shoot(float deltaTime, RealEngine::Entity entity) {
    RTypeProtocol::BaseMessage eventMessage;
    eventMessage.message_type = RTypeProtocol::MessageType::SHOOT_EVENT;

    std::array<char, 800> serializedEventMessage = RTypeProtocol::serialize<800>(eventMessage);
    _client->send_unreliable_packet(serializedEventMessage);
}

void Controls::holdShoot(float deltaTime, RealEngine::Entity entity) {
    auto* container = _registry.get_component<RealEngine::NetvarContainer>(entity);
    if (!container->getNetvar("hold_shoot")) {
        container->addNetvar("hold_shoot", RealEngine::Netvar{"float", "hold_shoot", 0.f, nullptr});
    }
    float hold_shoot = std::any_cast<float>(container->getNetvar("hold_shoot")->value);
    container->getNetvar("hold_shoot")->value = std::any_cast<float>(hold_shoot + deltaTime);
    auto* position = _registry.get_component<RealEngine::Position>(entity);
    _particlesTimer -= deltaTime;
    if (position && _particlesTimer <= 0) {
        ShootLoad(_registry, {position->x + 32.f, position->y + 7.5f});
        _particlesTimer = 0.1f;
    }
    RTypeProtocol::BaseMessage eventMessage;
    eventMessage.message_type = RTypeProtocol::MessageType::HOLD_SHOOT_EVENT;

    std::array<char, 800> serializedEventMessage = RTypeProtocol::serialize<800>(eventMessage);
    _client->send_unreliable_packet(serializedEventMessage);
}

void Controls::releaseShoot(float deltaTime, RealEngine::Entity entity) {
    auto* container = _registry.get_component<RealEngine::NetvarContainer>(entity);
    if (container) container->getNetvar("hold_shoot")->value = std::any_cast<float>(0.f);
    _registry.remove_component<RealEngine::ParticleEmitter>(entity);
    RTypeProtocol::BaseMessage eventMessage;
    eventMessage.message_type = RTypeProtocol::MessageType::RELEASE_SHOOT_EVENT;

    std::array<char, 800> serializedEventMessage = RTypeProtocol::serialize<800>(eventMessage);
    _client->send_unreliable_packet(serializedEventMessage);
}
}  // namespace rtype
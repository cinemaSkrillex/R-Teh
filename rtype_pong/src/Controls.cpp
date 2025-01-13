#include "Controls.hpp"

#include <RtypeServerProtocol.hpp>

#include "ECS/Registry/Registry.hpp"

namespace rtype {

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
    // std::cout << "HOLD SHOOT" << std::endl;
    RTypeProtocol::BaseMessage eventMessage;
    eventMessage.message_type = RTypeProtocol::MessageType::HOLD_SHOOT_EVENT;

    std::array<char, 800> serializedEventMessage = RTypeProtocol::serialize<800>(eventMessage);
    _client->send_unreliable_packet(serializedEventMessage);
}

void Controls::releaseShoot(float deltaTime, RealEngine::Entity entity) {
    // std::cout << "RELEASE SHOOT" << std::endl;
    RTypeProtocol::BaseMessage eventMessage;
    eventMessage.message_type = RTypeProtocol::MessageType::RELEASE_SHOOT_EVENT;

    std::array<char, 800> serializedEventMessage = RTypeProtocol::serialize<800>(eventMessage);
    _client->send_unreliable_packet(serializedEventMessage);
}
}  // namespace rtype
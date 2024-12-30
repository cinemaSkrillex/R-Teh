#include "Controls.hpp"

#include "ECS/Registry/Registry.hpp"

namespace rtype {

Controls::Controls(RealEngine::Registry& registry, std::shared_ptr<UDPClient> client)
    : _registry(registry), _client(client) {}

Controls::~Controls() {}

void Controls::moveUp(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                      RealEngine::Position& position, float deltaTime) {
    if (velocity.vy > 50) velocity.vy = 50;
    velocity.vy -= acceleration.ay * 3 * deltaTime;
}

void Controls::moveDown(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                        RealEngine::Position& position, float deltaTime) {
    if (velocity.vy < -50) velocity.vy = -50;
    velocity.vy += acceleration.ay * 3 * deltaTime;
}

void Controls::moveLeft(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                        RealEngine::Position& position, float deltaTime) {
    if (velocity.vx > 50) velocity.vx = 50;
    velocity.vx -= acceleration.ax * 3 * deltaTime;
}

void Controls::moveRight(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                         RealEngine::Position& position, float deltaTime) {
    if (velocity.vx < -50) velocity.vx = -50;
    velocity.vx += acceleration.ax * 3 * deltaTime;
}

void Controls::shoot(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                     RealEngine::Position& position, float deltaTime) {
    // _client->send_unreliable_packet("Event:Shoot");
}
}  // namespace rtype
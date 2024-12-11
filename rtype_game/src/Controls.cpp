#include "Controls.hpp"

#include "ECS/Registry/Registry.hpp"

namespace rtype {

Controls::Controls(RealEngine::Registry& registry)
    : _registry(registry),
      _shootCooldown(0.0f),
      _bulletSprite("../../assets/sprites/whanos.png", {0, 0, 524, 267}) {
    _bulletSprite.setScale(0.2, 0.2);
}

Controls::~Controls() { _entities.clear(); }

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
    RealEngine::Entity laserEntity = _registry.spawn_entity();
    _registry.add_component(laserEntity, RealEngine::AutoDestructible{1.0f});
    _registry.add_component(laserEntity, RealEngine::Position{position.x + 32 * 3, position.y});
    _registry.add_component(laserEntity, RealEngine::Velocity{200.0f, 0.0f, {200.0f, 0.0f}, 0.0f});
    _registry.add_component(laserEntity, RealEngine::Drawable{});
    _registry.add_component(laserEntity, RealEngine::SpriteComponent{_bulletSprite});
    _entities.push_back(new RealEngine::Entity(laserEntity));
    _shootCooldown = 0.5f;
}

}  // namespace rtype
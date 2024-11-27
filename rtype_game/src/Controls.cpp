#include "Controls.hpp"
#include "ECS/Registry/Registry.hpp"

namespace rtype {

Controls::Controls(RealEngine::Registry& registry) : _registry(registry) {}

Controls::~Controls() {
    _entities.clear();
    _sprites.clear();
}

void Controls::moveUp(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                      RealEngine::Position& position, float deltaTime) {
    if (velocity.vy > 50)
        velocity.vy = 50;
    velocity.vy -= acceleration.ay * 3 * deltaTime;
    if (velocity.vy < -400)
        velocity.vy = -400;
}

void Controls::moveDown(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                        RealEngine::Position& position, float deltaTime) {
    if (velocity.vy < -50)
        velocity.vy = -50;
    velocity.vy += acceleration.ay * 3 * deltaTime;
    if (velocity.vy > 400)
        velocity.vy = 400;
}

void Controls::moveLeft(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                        RealEngine::Position& position, float deltaTime) {
    if (velocity.vx > 50)
        velocity.vx = 50;
    velocity.vx -= acceleration.ax * 3 * deltaTime;
    if (velocity.vx < -400)
        velocity.vx = -400;
}

void Controls::moveRight(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                         RealEngine::Position& position, float deltaTime) {
    if (velocity.vx < -50)
        velocity.vx = -50;
    velocity.vx += acceleration.ax * 3 * deltaTime;
    if (velocity.vx > 400)
        velocity.vx = 400;
}

void Controls::decelerateUp(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                            RealEngine::Position& position, float deltaTime) {
    if (velocity.vy < 0) {
        velocity.vy += acceleration.ay * 3 * deltaTime;
        if (velocity.vy > 0)
            velocity.vy = 0;
    }
}

void Controls::decelerateDown(RealEngine::Velocity&     velocity,
                              RealEngine::Acceleration& acceleration,
                              RealEngine::Position& position, float deltaTime) {
    if (velocity.vy > 0) {
        velocity.vy -= acceleration.ay * 3 * deltaTime;
        if (velocity.vy < 0)
            velocity.vy = 0;
    }
}

void Controls::decelerateLeft(RealEngine::Velocity&     velocity,
                              RealEngine::Acceleration& acceleration,
                              RealEngine::Position& position, float deltaTime) {
    if (velocity.vx < 0) {
        velocity.vx += acceleration.ax * 3 * deltaTime;
        if (velocity.vx > 0)
            velocity.vx = 0;
    }
}

void Controls::decelerateRight(RealEngine::Velocity&     velocity,
                               RealEngine::Acceleration& acceleration,
                               RealEngine::Position& position, float deltaTime) {
    if (velocity.vx > 0) {
        velocity.vx -= acceleration.ax * 3 * deltaTime;
        if (velocity.vx < 0)
            velocity.vx = 0;
    }
}

void Controls::shoot(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                     RealEngine::Position& position, float deltaTime) {
    RealEngine::Entity laserEntity = _registry.spawn_entity();
    _registry.add_component(laserEntity, RealEngine::Position{position.x + 32 * 3, position.y});
    _registry.add_component(laserEntity, RealEngine::Velocity{200.0f, 0.0f});
    _registry.add_component(laserEntity, RealEngine::Drawable{});
    _sprites.push_back(new RealEngine::Sprite("../assets/sprites/whanos.png", {0, 0, 524, 267}));
    _sprites.back()->setScale(0.2, 0.2);
    _registry.add_component(laserEntity, RealEngine::SpriteComponent{*_sprites.back()});
    _entities.push_back(new RealEngine::Entity(laserEntity));
}

void Controls::voidAction(RealEngine::Velocity& velocity, RealEngine::Acceleration& acceleration,
                          RealEngine::Position& position, float deltaTime) {
    (void)velocity;
    (void)acceleration;
    (void)position;
    (void)deltaTime;
}
} // namespace rtype
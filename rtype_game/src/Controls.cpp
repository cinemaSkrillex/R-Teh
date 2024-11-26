#include "Controls.hpp"
#include "ECS/Registry/Registry.hpp"

namespace rtype {

Controls::Controls(Registry& registry) : _registry(registry) {}

Controls::~Controls() {
    _entities.clear();
    _sprites.clear();
}

void Controls::moveUp(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    if (velocity.vy > 50)
        velocity.vy = 50;
    velocity.vy -= acceleration.ay * 3 * deltaTime;
    if (velocity.vy < -400)
        velocity.vy = -400;
}

void Controls::moveDown(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    if (velocity.vy < -50)
        velocity.vy = -50;
    velocity.vy += acceleration.ay * 3 * deltaTime;
    if (velocity.vy > 400)
        velocity.vy = 400;
}

void Controls::moveLeft(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    if (velocity.vx > 50)
        velocity.vx = 50;
    velocity.vx -= acceleration.ax * 3 * deltaTime;
    if (velocity.vx < -400)
        velocity.vx = -400;
}

void Controls::moveRight(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    if (velocity.vx < -50)
        velocity.vx = -50;
    velocity.vx += acceleration.ax * 3 * deltaTime;
    if (velocity.vx > 400)
        velocity.vx = 400;
}

void Controls::decelerateUp(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    if (velocity.vy < 0) {
        velocity.vy += acceleration.ay * 3 * deltaTime;
        if (velocity.vy > 0)
            velocity.vy = 0;
    }
}

void Controls::decelerateDown(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    if (velocity.vy > 0) {
        velocity.vy -= acceleration.ay * 3 * deltaTime;
        if (velocity.vy < 0)
            velocity.vy = 0;
    }
}

void Controls::decelerateLeft(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    if (velocity.vx < 0) {
        velocity.vx += acceleration.ax * 3 * deltaTime;
        if (velocity.vx > 0)
            velocity.vx = 0;
    }
}

void Controls::decelerateRight(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    if (velocity.vx > 0) {
        velocity.vx -= acceleration.ax * 3 * deltaTime;
        if (velocity.vx < 0)
            velocity.vx = 0;
    }
}

void Controls::shoot(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    Entity laserEntity = _registry.spawn_entity();
    _registry.add_component(laserEntity, Position{100.f, 100.f});
    _registry.add_component(laserEntity, Velocity{200.0f, 0.0f});
    _registry.add_component(laserEntity, Drawable{});
    _sprites.push_back(new RealEngine::Sprite("../assets/sprites/whanos.png", {0, 0, 524, 267}));
    _sprites.back()->setScale(0.2, 0.2);
    _registry.add_component(laserEntity, Sprite{*_sprites.back()});
    _entities.push_back(new Entity(laserEntity));
}

void Controls::voidAction(Velocity& velocity, Acceleration& acceleration, float deltaTime) {
    (void)velocity;
    (void)acceleration;
    (void)deltaTime;
}
} // namespace rtype
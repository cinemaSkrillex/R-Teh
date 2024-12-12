/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** EyeBomber
*/

#include "EyeBomber.hpp"

namespace rtype {
EyeBomber::EyeBomber(RealEngine::Registry& registry, sf::Vector2f position,
                     RealEngine::Sprite eyeSprite)
    : _eyeEntity(registry.spawn_entity()),
      _eyeSprite(eyeSprite),
      _directionTimer(0.0f),
      _goLeft(false) {
    _eyeSprite.setScale(GAME_SCALE, GAME_SCALE);
    _eyeSheet.emplace("normal", _eyeSprite);

    registry.add_component(_eyeEntity, RealEngine::Position{position.x, position.y});
    registry.add_components(
        _eyeEntity,
        RealEngine::SpriteSheet{_eyeSheet, "normal", 0, {15, 10}, false, true, 120, {10, 5}},
        RealEngine::Drawable{});
    registry.add_component(_eyeEntity, RealEngine::Velocity{0.0f, 0.0f, {135.0f, 135.0f}, 0.8f});
    registry.add_component(_eyeEntity, RealEngine::Acceleration{60.0f, 5.0f, 0.5f});
    registry.add_component(_eyeEntity, RealEngine::Rotation{0.0f});
    registry.add_component(
        _eyeEntity,
        RealEngine::Collision{{0.f, 0.f, 15.f * GAME_SCALE, 10.f * GAME_SCALE},
                              "eye",
                              false,
                              RealEngine::CollisionType::HIT,
                              [this](RealEngine::CollisionType collisionType,
                                     RealEngine::Registry& registry, RealEngine::Entity collider) {
                                  collisionBehaviour(collisionType, registry, collider);
                              }});
    registry.add_component(_eyeEntity, RealEngine::Damage{10});
    registry.add_component(_eyeEntity, RealEngine::Health{50, 50});
    registry.add_component(
        _eyeEntity,
        RealEngine::AI{[this](RealEngine::Registry& registry, RealEngine::Entity target,
                              float deltaTime) { agressiveBehavior(registry, target, deltaTime); },
                       [this](RealEngine::Registry& registry, float deltaTime) {
                           simpleBehavior(registry, deltaTime);
                       },
                       true});
}

EyeBomber::EyeBomber(RealEngine::Registry& registry, sf::Vector2f position)
    : _eyeEntity(registry.spawn_entity()), _directionTimer(0.0f), _goLeft(false) {
    registry.add_component(_eyeEntity, RealEngine::Position{position.x, position.y});
    registry.add_component(_eyeEntity, RealEngine::Velocity{0.0f, 0.0f, {135.0f, 135.0f}, 0.8f});
    registry.add_component(_eyeEntity, RealEngine::Acceleration{240.0f, 240.0f, 2.0f});
    registry.add_component(_eyeEntity, RealEngine::Rotation{0.0f});
    registry.add_component(
        _eyeEntity,
        RealEngine::AI{[this](RealEngine::Registry& registry, RealEngine::Entity target,
                              float deltaTime) { agressiveBehavior(registry, target, deltaTime); },
                       [this](RealEngine::Registry& registry, float deltaTime) {
                           simpleBehavior(registry, deltaTime);
                       },
                       true});
}

EyeBomber::~EyeBomber() {}

void EyeBomber::setTarget(RealEngine::Entity target, RealEngine::Registry& registry) {
    auto* acceleration = registry.get_component<RealEngine::Acceleration>(_eyeEntity);

    acceleration->ax = 240.0f;
    acceleration->ay = 240.0f;
    registry.add_component(_eyeEntity, RealEngine::Target{target});
}

void EyeBomber::agressiveBehavior(RealEngine::Registry& registry, RealEngine::Entity target,
                                  float deltaTime) {
    auto* position       = registry.get_component<RealEngine::Position>(_eyeEntity);
    auto* acceleration   = registry.get_component<RealEngine::Acceleration>(_eyeEntity);
    auto* velocity       = registry.get_component<RealEngine::Velocity>(_eyeEntity);
    auto* rotation       = registry.get_component<RealEngine::Rotation>(_eyeEntity);
    auto* targetPosition = registry.get_component<RealEngine::Position>(target);

    aimAtTarget(targetPosition, registry, 2.5f, deltaTime);
    if (position && targetPosition && acceleration) {
        float dx       = targetPosition->x - position->x;
        float dy       = targetPosition->y - position->y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 10.0f) {
            velocity->vx += acceleration->ax * std::cos(rotation->angle * M_PI / 180.0f);
            velocity->vy += acceleration->ay * std::sin(rotation->angle * M_PI / 180.0f);
        }
    }
}

void EyeBomber::aimAtTarget(RealEngine::Position* targetPosition, RealEngine::Registry& registry,
                            float rotationSpeed, float deltaTime) {
    auto* position = registry.get_component<RealEngine::Position>(_eyeEntity);
    auto* rotation = registry.get_component<RealEngine::Rotation>(_eyeEntity);

    if (position && targetPosition) {
        float dx          = targetPosition->x - position->x;
        float dy          = targetPosition->y - position->y;
        float targetAngle = std::atan2(dy, dx) * 180.0f / M_PI;

        if (rotation) {
            float currentAngle    = rotation->angle;
            targetAngle           = std::fmod(targetAngle + 360.0f, 360.0f);
            currentAngle          = std::fmod(currentAngle + 360.0f, 360.0f);
            float angleDifference = targetAngle - currentAngle;

            if (angleDifference > 180.0f) {
                angleDifference -= 360.0f;
            } else if (angleDifference < -180.0f) {
                angleDifference += 360.0f;
            }
            if (angleDifference > 0.1f) {
                rotation->angle += rotationSpeed * deltaTime * 100.0f;
            } else if (angleDifference < -0.1f) {
                rotation->angle -= rotationSpeed * deltaTime * 100.0f;
            }
            rotation->angle = std::fmod(rotation->angle + 360.0f, 360.0f);
        }
    }
}

void EyeBomber::simpleBehavior(RealEngine::Registry& registry, float deltaTime) {
    auto* eyeRotation     = registry.get_component<RealEngine::Rotation>(_eyeEntity);
    auto* eyeVelocity     = registry.get_component<RealEngine::Velocity>(_eyeEntity);
    auto* eyeAcceleration = registry.get_component<RealEngine::Acceleration>(_eyeEntity);

    eyeVelocity->vx += eyeAcceleration->ax * deltaTime;
    eyeVelocity->vy += eyeAcceleration->ay * deltaTime;
    _directionTimer -= deltaTime;
    if (_directionTimer <= 0.0f) {
        eyeVelocity->vx     = 0.0f;
        eyeVelocity->vy     = 0.0f;
        eyeAcceleration->ax = -eyeAcceleration->ax;
        eyeAcceleration->ay = -eyeAcceleration->ay;
        _goLeft             = !_goLeft;
        eyeRotation->angle += 180.0f;
        _directionTimer = 1.8f;
    }
}

void EyeBomber::collisionBehaviour(RealEngine::CollisionType collisionType,
                                   RealEngine::Registry& registry, RealEngine::Entity collider) {
    switch (collisionType) {
        case RealEngine::CollisionType::INACTIVE:
            break;
        case RealEngine::CollisionType::SOLID:
            break;
        case RealEngine::CollisionType::HIT:
            break;
        case RealEngine::CollisionType::PICKABLE:
            break;
        case RealEngine::CollisionType::OTHER:
            selfDestruct(registry);
            break;
        default:
            break;
    }
}

void EyeBomber::selfDestruct(RealEngine::Registry& registry) {
    auto* health = registry.get_component<RealEngine::Health>(_eyeEntity);
    if (health) health->damage += health->maxHealth;
}
}  // namespace rtype
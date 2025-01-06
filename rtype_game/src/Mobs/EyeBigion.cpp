/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeBigion
*/

#include "Mobs/EyeBigion.hpp"

namespace rtype {

static void aimAtTarget(RealEngine::Entity& entity, RealEngine::Position* targetPosition,
                        RealEngine::Registry& registry, float rotationSpeed, float deltaTime) {
    auto* position = registry.get_component<RealEngine::Position>(entity);
    auto* rotation = registry.get_component<RealEngine::Rotation>(entity);

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

static void agressiveBehavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                              float deltaTime) {
    auto  entity_target  = registry.get_component<RealEngine::Target>(entity);
    auto* position       = registry.get_component<RealEngine::Position>(entity);
    auto* acceleration   = registry.get_component<RealEngine::Acceleration>(entity);
    auto* velocity       = registry.get_component<RealEngine::Velocity>(entity);
    auto* rotation       = registry.get_component<RealEngine::Rotation>(entity);
    auto* targetPosition = registry.get_component<RealEngine::Position>(entity_target->target);

    aimAtTarget(entity, targetPosition, registry, 2.5f, deltaTime);
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

static void simpleBehavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                           float deltaTime) {
    auto* eyeRotation     = registry.get_component<RealEngine::Rotation>(entity);
    auto* eyeVelocity     = registry.get_component<RealEngine::Velocity>(entity);
    auto* eyeAcceleration = registry.get_component<RealEngine::Acceleration>(entity);

    if (!eyeRotation || !eyeVelocity || !eyeAcceleration)
        std::cout << "Error: EyeBigion components not found!" << std::endl;
}

EyeBigion::EyeBigion(RealEngine::Registry& registry, sf::Vector2f position, sf::Vector2f direction,
                     float speed, RealEngine::Sprite& normalMobSprite,
                     RealEngine::Sprite& angryMobSprite)
    : _eyeEntity(registry.spawn_entity()) {
    _eyeSheet.emplace("normal", normalMobSprite);
    _eyeSheet.emplace("angry", angryMobSprite);

    registry.add_component(_eyeEntity, RealEngine::Position{position.x, position.y});
    registry.add_components(
        _eyeEntity,
        RealEngine::SpriteSheet{
            _eyeSheet, "normal", 0, {23, 16}, false, true, 120, {17, 8}, sf::Clock()},
        RealEngine::Drawable{});
    // 23, 16 size and 17, 8 center for normal form
    // 21, 16 size and 16, 8 center for angry form
    registry.add_component(_eyeEntity, RealEngine::Velocity{0.0f, 0.0f, {135.0f, 135.0f}, 0.8f});
    registry.add_component(_eyeEntity, RealEngine::Acceleration{60.0f, 5.0f, 0.5f});
    registry.add_component(_eyeEntity, RealEngine::Rotation{0.0f});
    registry.add_component(
        _eyeEntity,
        RealEngine::Collision{
            {0.0f, 0.0f, 15.f * GAME_SCALE, 10.f * GAME_SCALE},
            "mob",
            false,
            RealEngine::CollisionType::ENEMY,
            [this](RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                   RealEngine::Entity collider, RealEngine::Entity entity) {
                collisionBehaviour(collisionType, registry, collider, entity);
            }});
    registry.add_component(_eyeEntity, RealEngine::Health{50, 50});
    registry.add_component(_eyeEntity, RealEngine::AI{agressiveBehavior, simpleBehavior, true});
    registry.add_component(_eyeEntity, RealEngine::Damage{40});
}

EyeBigion::~EyeBigion() {}

void EyeBigion::setTarget(std::shared_ptr<RealEngine::Entity> target,
                          RealEngine::Registry&               registry) {
    auto* acceleration = registry.get_component<RealEngine::Acceleration>(_eyeEntity);

    acceleration->ax = 240.0f;
    acceleration->ay = 240.0f;
    registry.add_component(_eyeEntity, RealEngine::Target{target});
}

static void selfDestruct(RealEngine::Registry& registry, RealEngine::Entity entity) {
    auto* health = registry.get_component<RealEngine::Health>(entity);

    if (health) {
        health->damage = health->maxHealth;
    }
}

void EyeBigion::collisionBehaviour(RealEngine::CollisionType collisionType,
                                   RealEngine::Registry& registry, RealEngine::Entity collider,
                                   RealEngine::Entity entity) {
    switch (collisionType) {
        case RealEngine::CollisionType::INACTIVE:
            break;
        case RealEngine::CollisionType::SOLID:
            break;
        case RealEngine::CollisionType::ENEMY:
            break;
        case RealEngine::CollisionType::PICKABLE:
            break;
        case RealEngine::CollisionType::PLAYER:
            selfDestruct(registry, entity);
            break;
        default:
            break;
    }
}

}  // namespace rtype
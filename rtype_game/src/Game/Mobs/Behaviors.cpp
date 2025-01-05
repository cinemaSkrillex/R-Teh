/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Behaviors
*/

#include "Game/Mobs/Behaviors.hpp"

namespace rtype {

void noBehavior(RealEngine::Registry& registry, RealEngine::Entity entity, float deltaTime) {
    (void)registry;
    (void)entity;
    (void)deltaTime;
}

void aimAtTarget(RealEngine::Entity& entity, RealEngine::Position* targetPosition,
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

void rushTowardsTarget(RealEngine::Registry& registry, RealEngine::Entity entity, float deltaTime) {
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
            float accelerationX = dx / distance * 6.0f;
            float accelerationY = dy / distance * 6.0f;
            velocity->vx += accelerationX * deltaTime;
            velocity->vy += accelerationY * deltaTime;
        }
    }
}

void rushTowardsTargetZigzagging(RealEngine::Registry& registry, RealEngine::Entity entity,
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

void goStraightAngle(RealEngine::Registry& registry, RealEngine::Entity entity, float deltaTime) {
    auto* rotation     = registry.get_component<RealEngine::Rotation>(entity);
    auto* acceleration = registry.get_component<RealEngine::Acceleration>(entity);
    auto* velocity     = registry.get_component<RealEngine::Velocity>(entity);

    float angleRad = rotation->angle * (M_PI / 180.0f);
    velocity->vx += (std::cos(angleRad) * acceleration->ax) * deltaTime;
    velocity->vy += (std::sin(angleRad) * acceleration->ay) * deltaTime;
}

void goStraight(RealEngine::Registry& registry, RealEngine::Entity entity, float deltaTime) {
    auto* acceleration = registry.get_component<RealEngine::Acceleration>(entity);
    auto* velocity     = registry.get_component<RealEngine::Velocity>(entity);

    velocity->vx += acceleration->ax * deltaTime;
    velocity->vy += acceleration->ay * deltaTime;
}

void noCollisionBehavior(RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                         RealEngine::Entity collider, RealEngine::Entity entity) {
    switch (collisionType) {
        case RealEngine::CollisionType::INACTIVE:
            break;
        case RealEngine::CollisionType::SOLID:
            break;
        case RealEngine::CollisionType::PICKABLE:
            break;
        case RealEngine::CollisionType::OTHER:
            break;
        case RealEngine::CollisionType::PLAYER:
            break;
        case RealEngine::CollisionType::ENEMY:
            break;
        case RealEngine::CollisionType::ALLY_BULLET:
            break;
        case RealEngine::CollisionType::ENEMY_BULLET:
            break;
        default:
            break;
    }
}

void destroyOnWallsAndPlayer_TakesDamage(RealEngine::CollisionType collisionType,
                                         RealEngine::Registry&     registry,
                                         RealEngine::Entity collider, RealEngine::Entity entity) {
    switch (collisionType) {
        case RealEngine::CollisionType::INACTIVE:
            break;
        case RealEngine::CollisionType::SOLID:
            selfDestruct(registry, entity);
            break;
        case RealEngine::CollisionType::PICKABLE:
            break;
        case RealEngine::CollisionType::OTHER:
            break;
        case RealEngine::CollisionType::PLAYER:
            selfDestruct(registry, entity);
            break;
        case RealEngine::CollisionType::ENEMY:
            break;
        case RealEngine::CollisionType::ALLY_BULLET:
            takeDamageFromCollider(registry, collider, entity);
            break;
        case RealEngine::CollisionType::ENEMY_BULLET:
            break;
        default:
            break;
    }
}

void destroyOnWallsAndPlayer(RealEngine::CollisionType collisionType,
                             RealEngine::Registry& registry, RealEngine::Entity collider,
                             RealEngine::Entity entity) {
    switch (collisionType) {
        case RealEngine::CollisionType::INACTIVE:
            break;
        case RealEngine::CollisionType::SOLID:
            selfDestruct(registry, entity);
            break;
        case RealEngine::CollisionType::PICKABLE:
            break;
        case RealEngine::CollisionType::OTHER:
            break;
        case RealEngine::CollisionType::PLAYER:
            selfDestruct(registry, entity);
            break;
        case RealEngine::CollisionType::ENEMY:
            break;
        case RealEngine::CollisionType::ALLY_BULLET:
            break;
        case RealEngine::CollisionType::ENEMY_BULLET:
            break;
        default:
            break;
    }
}

void takesDamage(RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                 RealEngine::Entity collider, RealEngine::Entity entity) {
    switch (collisionType) {
        case RealEngine::CollisionType::INACTIVE:
            break;
        case RealEngine::CollisionType::SOLID:
            break;
        case RealEngine::CollisionType::PICKABLE:
            break;
        case RealEngine::CollisionType::OTHER:
            break;
        case RealEngine::CollisionType::PLAYER:
            break;
        case RealEngine::CollisionType::ENEMY:
            break;
        case RealEngine::CollisionType::ALLY_BULLET:
            takeDamageFromCollider(registry, collider, entity);
            break;
        case RealEngine::CollisionType::ENEMY_BULLET:
            break;
        default:
            break;
    }
}

void takeDamageFromCollider(RealEngine::Registry& registry, RealEngine::Entity collider,
                            RealEngine::Entity entity) {
    auto* health = registry.get_component<RealEngine::Health>(entity);
    auto* damage = registry.get_component<RealEngine::Damage>(collider);

    if (health && damage) {
        if (damage->effect) {
            health->regenerationRate     = -damage->amount;
            health->regenerationCooldown = damage->effectInterval;
            health->regenerationTimer    = 0.0f;
            health->regenerationTime     = damage->effectDuration;
        } else {
            health->damage += damage->amount;
        }
    }
}

void selfDestruct(RealEngine::Registry& registry, RealEngine::Entity entity) {
    auto* health = registry.get_component<RealEngine::Health>(entity);

    if (health) {
        health->damage = health->maxHealth;
    }
}

}  // namespace rtype
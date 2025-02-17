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
    auto* position = registry.getComponent<RealEngine::Position>(entity);
    auto* rotation = registry.getComponent<RealEngine::Rotation>(entity);

    if (position && targetPosition) {
        float dx          = targetPosition->x - position->x;
        float dy          = targetPosition->y - position->y;
        float targetAngle = std::atan2(dy, dx) * 180.0f / M_PI;
        float maxRotation = (rotationSpeed * deltaTime) * 100.0f;

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

            if (std::abs(angleDifference) < maxRotation) {
                rotation->angle = targetAngle;
            } else {
                rotation->angle += (angleDifference > 0 ? 1 : -1) * maxRotation;
            }

            rotation->angle = std::fmod(rotation->angle + 360.0f, 360.0f);
        }
    }
}

void rushAndAimTowardsTarget(RealEngine::Registry& registry, RealEngine::Entity entity,
                             float deltaTime) {
    auto  entity_target  = registry.getComponent<RealEngine::Target>(entity);
    auto* position       = registry.getComponent<RealEngine::Position>(entity);
    auto* acceleration   = registry.getComponent<RealEngine::Acceleration>(entity);
    auto* velocity       = registry.getComponent<RealEngine::Velocity>(entity);
    auto* rotation       = registry.getComponent<RealEngine::Rotation>(entity);
    auto* targetPosition = registry.getComponent<RealEngine::Position>(entity_target->target);

    aimAtTarget(entity, targetPosition, registry, 2.5f, deltaTime);
    if (position && targetPosition && acceleration) {
        float dx       = targetPosition->x - position->x;
        float dy       = targetPosition->y - position->y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 10.0f) {
            float accelerationX = dx / distance * 6.0f;
            float accelerationY = dy / distance * 6.0f;
            velocity->vx += (accelerationX * deltaTime) * 100.0f;
            velocity->vy += (accelerationY * deltaTime) * 100.0f;
        }
    }
}

void rushAndAimTowardsTargetZigzagging(RealEngine::Registry& registry, RealEngine::Entity entity,
                                       float deltaTime) {
    auto  entity_target  = registry.getComponent<RealEngine::Target>(entity);
    auto* position       = registry.getComponent<RealEngine::Position>(entity);
    auto* acceleration   = registry.getComponent<RealEngine::Acceleration>(entity);
    auto* velocity       = registry.getComponent<RealEngine::Velocity>(entity);
    auto* rotation       = registry.getComponent<RealEngine::Rotation>(entity);
    auto* targetPosition = registry.getComponent<RealEngine::Position>(entity_target->target);

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

void rushTowardsTarget(RealEngine::Registry& registry, RealEngine::Entity entity, float deltaTime) {
    auto  entity_target  = registry.getComponent<RealEngine::Target>(entity);
    auto* position       = registry.getComponent<RealEngine::Position>(entity);
    auto* acceleration   = registry.getComponent<RealEngine::Acceleration>(entity);
    auto* velocity       = registry.getComponent<RealEngine::Velocity>(entity);
    auto* rotation       = registry.getComponent<RealEngine::Rotation>(entity);
    auto* targetPosition = registry.getComponent<RealEngine::Position>(entity_target->target);

    if (position && targetPosition && acceleration) {
        float dx       = targetPosition->x - position->x;
        float dy       = targetPosition->y - position->y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 10.0f) {
            float accelerationX = dx / distance * 6.0f;
            float accelerationY = dy / distance * 6.0f;
            velocity->vx += (accelerationX * deltaTime) * 100.0f;
            velocity->vy += (accelerationY * deltaTime) * 100.0f;
        }
    }
}

void goStraightAngle(RealEngine::Registry& registry, RealEngine::Entity entity, float deltaTime) {
    auto* position     = registry.getComponent<RealEngine::Position>(entity);
    auto* velocity     = registry.getComponent<RealEngine::Velocity>(entity);
    auto* rotation     = registry.getComponent<RealEngine::Rotation>(entity);
    auto* acceleration = registry.getComponent<RealEngine::Acceleration>(entity);

    if (position && velocity && rotation && acceleration) {
        float angleRad = rotation->angle * (M_PI / 180.0f);

        float targetX = position->x + std::cos(angleRad) * acceleration->ax;
        float targetY = position->y + std::sin(angleRad) * acceleration->ay;

        float dx       = targetX - position->x;
        float dy       = targetY - position->y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 10.0f) {
            float accelerationX = dx / distance * 6.0f;
            float accelerationY = dy / distance * 6.0f;
            std::cout << "angleRad: " << rotation->angle << std::endl;
            std::cout << "accelerationX: " << accelerationX << " accelerationY: " << accelerationY
                      << std::endl;

            velocity->vx += (accelerationX * deltaTime) * 100.0f;
            velocity->vy += (accelerationY * deltaTime) * 100.0f;
        }
    }
}

void goStraight(RealEngine::Registry& registry, RealEngine::Entity entity, float deltaTime) {
    auto* acceleration = registry.getComponent<RealEngine::Acceleration>(entity);
    auto* velocity     = registry.getComponent<RealEngine::Velocity>(entity);

    if (acceleration && velocity) {
        velocity->vx = (acceleration->ax * deltaTime) * 100.0f;
        velocity->vy = (acceleration->ay * deltaTime) * 100.0f;
    }
}

void goStraightConstant(RealEngine::Registry& registry, RealEngine::Entity entity,
                        float deltaTime) {
    auto* acceleration = registry.getComponent<RealEngine::Acceleration>(entity);
    auto* position     = registry.getComponent<RealEngine::Position>(entity);

    if (position && acceleration) {
        position->x += acceleration->ax * deltaTime;
        position->y += acceleration->ay * deltaTime;
    }
}

void goStraightConstantAngle(RealEngine::Registry& registry, RealEngine::Entity entity,
                             float deltaTime) {
    auto* acceleration = registry.getComponent<RealEngine::Acceleration>(entity);
    auto* position     = registry.getComponent<RealEngine::Position>(entity);
    auto* rotation     = registry.getComponent<RealEngine::Rotation>(entity);

    if (position && acceleration && rotation) {
        float angleRad = rotation->angle * (M_PI / 180.0f);

        position->x += std::cos(angleRad) * acceleration->ax * deltaTime;
        position->y += std::sin(angleRad) * acceleration->ay * deltaTime;
    }
}

void moveWithoutVelocity(RealEngine::Registry& registry, RealEngine::Entity entity,
                         float deltaTime) {
    auto* position = registry.getComponent<RealEngine::Position>(entity);
    auto* velocity = registry.getComponent<RealEngine::Velocity>(entity);

    if (position && velocity) {
        position->x += velocity->vx * deltaTime;
        position->y += velocity->vy * deltaTime;
    }
}

void destroyOutOfScreen(RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                        RealEngine::Entity entity) {
    auto* autoDestructible = registry.getComponent<RealEngine::AutoDestructible>(entity);

    if (collisionType != RealEngine::CollisionType::SCREEN) return;
    if (!autoDestructible) {
        auto* container = registry.getComponent<RealEngine::NetvarContainer>(entity);
        if (container) {
            if (container->getNetvar("destroy_out_of_screen") &&
                std::any_cast<bool>(container->getNetvar("destroy_out_of_screen")->value) ==
                    false) {
                registry.addComponent(entity, RealEngine::AutoDestructible{-1.0f, true, false});
            }
        }
    } else {
        autoDestructible->kill = false;
    }
}

void noCollisionBehavior(RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                         RealEngine::Entity collider, RealEngine::Entity entity) {
    destroyOutOfScreen(collisionType, registry, entity);
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

void destroyOnWalls(RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                    RealEngine::Entity collider, RealEngine::Entity entity) {
    destroyOutOfScreen(collisionType, registry, entity);
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
    destroyOutOfScreen(collisionType, registry, entity);
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
    destroyOutOfScreen(collisionType, registry, entity);
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
    destroyOutOfScreen(collisionType, registry, entity);
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
    auto* health = registry.getComponent<RealEngine::Health>(entity);
    auto* damage = registry.getComponent<RealEngine::Damage>(collider);

    if (health && damage && health->invincibilityTime <= 0.0f) {
        if (damage->effect) {
            health->regenerationRate     = -damage->amount;
            health->regenerationCooldown = damage->effectInterval;
            health->regenerationTimer    = 0.0f;
            health->regenerationTime     = damage->effectDuration;
        } else {
            health->amount -= damage->amount;
        }
        health->lastDamager = collider;
    }
}

void selfDestruct(RealEngine::Registry& registry, RealEngine::Entity entity) {
    auto* health = registry.getComponent<RealEngine::Health>(entity);

    if (health) {
        health->amount -= health->maxHealth;
    } else {
        registry.addComponent(entity, RealEngine::AutoDestructible{0.0f});
    }
}

void blockEntity(RealEngine::Registry& registry, RealEngine::Entity collider,
                 RealEngine::Entity entity) {
    auto* playerCollision   = registry.getComponent<RealEngine::Collision>(entity);
    auto* colliderCollision = registry.getComponent<RealEngine::Collision>(collider);
    auto* entityPosition    = registry.getComponent<RealEngine::Position>(entity);

    if (playerCollision && colliderCollision) {
        sf::FloatRect entityBounds   = playerCollision->bounds;
        sf::FloatRect colliderBounds = colliderCollision->bounds;

        float overlapLeft   = (entityBounds.left + entityBounds.width) - colliderBounds.left;
        float overlapRight  = (colliderBounds.left + colliderBounds.width) - entityBounds.left;
        float overlapTop    = (entityBounds.top + entityBounds.height) - colliderBounds.top;
        float overlapBottom = (colliderBounds.top + colliderBounds.height) - entityBounds.top;

        float correctionX = (overlapLeft < overlapRight) ? -overlapLeft : overlapRight;
        float correctionY = (overlapTop < overlapBottom) ? -overlapTop : overlapBottom;

        if (std::abs(correctionX) < std::abs(correctionY)) {
            entityPosition->x += correctionX;
        } else {
            entityPosition->y += correctionY;
        }
    }
}

}  // namespace rtype
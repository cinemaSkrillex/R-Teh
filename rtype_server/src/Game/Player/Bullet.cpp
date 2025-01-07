/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Bullet
*/

#include "Game/Player/Bullet.hpp"

namespace rtype {

static void destroyOnWallsAndEnemies(RealEngine::CollisionType collisionType,
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
            break;
        case RealEngine::CollisionType::ENEMY:
            // selfDestruct(registry, entity);
            // bulletTakesDamage(collisionType, registry, collider, entity);
            break;
        case RealEngine::CollisionType::ALLY_BULLET:
            break;
        case RealEngine::CollisionType::ENEMY_BULLET:
            break;
        default:
            break;
    }
}

// no more direction, the bullet will always go in the direction of its angle
Bullet::Bullet(RealEngine::Registry& registry, sf::Vector2f position, sf::Vector2f direction,
               float speed)
    : _entity(registry.spawn_entity()) {
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(
        _entity,
        RealEngine::Velocity{direction.x * speed, direction.y * speed, {500.f, 500.f}, 0.f});
    registry.add_component(_entity,
                           RealEngine::SpriteComponent{
                               *(RealEngine::AssetManager::getInstance().getSprite("bullet"))});
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(_entity,
                           RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                 "bullet",
                                                 false,
                                                 RealEngine::CollisionType::ALLY_BULLET,
                                                 destroyOnWallsAndEnemies});
    registry.add_component(_entity, RealEngine::AutoDestructible{5});
    registry.add_component(_entity, RealEngine::Damage{10});
    registry.add_component(_entity, RealEngine::Health{10, 20});
}

Bullet::~Bullet() {}
}  // namespace rtype
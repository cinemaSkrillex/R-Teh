/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Bullet
*/

#include "Game/Player/Bullet.hpp"

namespace rtype {

static void bulletTakesDamage(RealEngine::CollisionType collisionType,
                              RealEngine::Registry& registry, RealEngine::Entity collider,
                              RealEngine::Entity entity) {
    auto* health         = registry.get_component<RealEngine::Health>(entity);
    auto* colliderHealth = registry.get_component<RealEngine::Health>(collider);
    auto* damage         = registry.get_component<RealEngine::Damage>(entity);

    if ((colliderHealth && damage) && (colliderHealth->amount > damage->amount)) {
        selfDestruct(registry, entity);
    }
}

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
            bulletTakesDamage(collisionType, registry, collider, entity);
            break;
        case RealEngine::CollisionType::ALLY_BULLET:
            break;
        case RealEngine::CollisionType::ENEMY_BULLET:
            break;
        default:
            break;
    }
}

Bullet::Bullet(RealEngine::Registry& registry, sf::Vector2f position, sf::Vector2f direction,
               float speed, std::string spriteName, float damage, int health)
    : _entity(registry.spawn_entity()) {
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(
        _entity,
        RealEngine::Velocity{direction.x * speed, direction.y * speed, {500.f, 500.f}, 0.f});
    registry.add_component(_entity,
                           RealEngine::SpriteComponent{
                               *(RealEngine::AssetManager::getInstance().getSprite(spriteName))});
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(
        _entity,
        RealEngine::Collision{{-1.f, -1.f, -1.f, -1.f},  // automatic adjustement of collision
                              "bullet",
                              false,
                              RealEngine::CollisionType::ALLY_BULLET,
                              destroyOnWallsAndEnemies});
    registry.add_component(_entity, RealEngine::AutoDestructible{5});
    registry.add_component(_entity, RealEngine::Damage{damage});
    registry.add_component(_entity, RealEngine::Health{health, health});
}

Bullet::~Bullet() {}
}  // namespace rtype
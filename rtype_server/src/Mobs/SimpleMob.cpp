/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** SimpleMob
*/

#include "../include/Game/SimpleMob.hpp"

namespace rtype {

SimpleMob::SimpleMob(RealEngine::Registry& registry, sf::Vector2f position, sf::Vector2f direction,
                     float speed, float destructTimer, RealEngine::Sprite& mobSprite)
    : _entity(registry.spawn_entity()), _mobSprite(mobSprite) {
    _mobSpriteSheet.emplace("idle", mobSprite);
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(
        _entity,
        RealEngine::Velocity{direction.x * speed, direction.y * speed, {500.f, 500.f}, 0.f});
    // registry.add_component(
    //     _entity,
    //     RealEngine::SpriteSheet{_mobSpriteSheet, "idle", 0, {15, 10}, false, true, 100, {11,
    //     4}});
    registry.add_component(_entity, RealEngine::SpriteComponent{_mobSprite});
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(
        _entity, RealEngine::Collision{
                     {0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                     "mob",
                     false,
                     RealEngine::CollisionType::ENEMY,
                     [this](RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                            RealEngine::Entity collider, RealEngine::Entity entity) {
                         mob_collision_handler(collisionType, registry, collider, entity);
                     }});
    registry.add_component(_entity, RealEngine::AutoDestructible{destructTimer});
    registry.add_component(_entity, RealEngine::Damage{10});
    registry.add_component(_entity, RealEngine::Health{10, 10});
    registry.add_component(_entity, RealEngine::Rotation{180.f});
    registry.add_component(_entity, RealEngine::Netvar{"MOB", "dropChance", 33.0f});
}

SimpleMob::~SimpleMob() {}

void SimpleMob::mob_collision_handler(RealEngine::CollisionType collisionType,
                                      RealEngine::Registry& registry, RealEngine::Entity collider,
                                      RealEngine::Entity entity) {
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
            mob_take_damage(registry, collider, entity);
            break;
        case RealEngine::CollisionType::ALLY_BULLET:
            mob_take_damage(registry, collider, entity);
            break;
        case RealEngine::CollisionType::ENEMY_BULLET:
            break;
        default:
            break;
    }
}

void SimpleMob::mob_take_damage(RealEngine::Registry& registry, RealEngine::Entity collider,
                                RealEngine::Entity entity) {
    auto* health       = registry.get_component<RealEngine::Health>(entity);
    auto* bulletHealth = registry.get_component<RealEngine::Health>(collider);

    if (health) {
        health->damage += 10;
    }
    if (bulletHealth) {
        bulletHealth->damage += 10;
    }
}
}  // namespace rtype
/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SpacePlane
*/

#include "Mobs/SpacePlane.hpp"

namespace rtype {

static void straight_line_behavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                                   float deltaTime) {
    auto* position = registry.get_component<RealEngine::Position>(entity);
    auto* velocity = registry.get_component<RealEngine::Velocity>(entity);

    if (position->x > 0) {
        velocity->vx = -100.0f;
    }
}

static void agressive_behavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                               float deltaTime) {
    // no agressive behavior
}

SpacePlane::SpacePlane(RealEngine::Registry& registry, sf::Vector2f position,
                       sf::Vector2f direction, float speed)
    : _entity(registry.spawn_entity()),
      _mobSprite(*(RealEngine::AssetManager::getInstance().getSprite("space_plane"))) {
    _mobSpriteSheet.emplace("normal", _mobSprite);
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{speed, 0, {500.f, 500.f}, 0.5f});
    registry.add_component(
        _entity,
        RealEngine::SpriteSheet{
            _mobSpriteSheet, "normal", 0, {21, 23}, false, true, 75, {10, 12}, sf::Clock()});
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
    registry.add_component(_entity,
                           RealEngine::AI{agressive_behavior, straight_line_behavior, true});
    registry.add_component(_entity, RealEngine::Damage{50});
    registry.add_component(_entity, RealEngine::Health{40, 40});
    registry.add_component(_entity, RealEngine::Rotation{0.f});
}

SpacePlane::~SpacePlane() {}

void SpacePlane::mob_collision_handler(RealEngine::CollisionType collisionType,
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

void SpacePlane::mob_take_damage(RealEngine::Registry& registry, RealEngine::Entity collider,
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
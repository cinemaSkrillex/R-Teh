/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SimpleShoot
*/

#include "Mobs/Projectiles/SimpleShoot.hpp"

namespace rtype {

static void straight_line_behavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                                   float deltaTime) {
    auto* position = registry.get_component<RealEngine::Position>(entity);
    auto* velocity = registry.get_component<RealEngine::Velocity>(entity);
}

static void agressive_behavior(RealEngine::Registry& registry, RealEngine::Entity entity,
                               float deltaTime) {
    // no agressive behavior
}

SimpleShoot::SimpleShoot(RealEngine::Registry& registry, sf::Vector2f position, float angle,
                         float speed, RealEngine::Sprite& projSprite)
    : _entity(registry.spawn_entity()), _projSprite(projSprite) {
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{speed, 0, {850.f, 850.f}, 0.5f});
    registry.add_component(_entity, RealEngine::SpriteComponent{_projSprite});
    registry.add_component(_entity, RealEngine::Drawable{});
    // registry.add_component(
    //     _entity, RealEngine::Collision{
    //                  {0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
    //                  "mob",
    //                  false,
    //                  RealEngine::CollisionType::ENEMY,
    //                  [this](RealEngine::CollisionType collisionType, RealEngine::Registry&
    //                  registry,
    //                         RealEngine::Entity collider, RealEngine::Entity entity) {
    //                      mob_collision_handler(collisionType, registry, collider, entity);
    //                  }});
    registry.add_component(_entity,
                           RealEngine::AI{agressive_behavior, straight_line_behavior, true});
    registry.add_component(_entity, RealEngine::Damage{50});
    registry.add_component(_entity, RealEngine::Health{40, 40});
    registry.add_component(_entity, RealEngine::Rotation{0.f});
}

SimpleShoot::~SimpleShoot() {}

}  // namespace rtype
/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SpaceSphere
*/

#include "Mobs/SpaceSphere.hpp"

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

SpaceSphere::SpaceSphere(RealEngine::Registry& registry, sf::Vector2f position,
                         sf::Vector2f direction, float speed, RealEngine::Sprite& mobSprite)
    : _entity(registry.spawn_entity()), _mobSprite(mobSprite) {
    _mobSpriteSheet.emplace("fly", mobSprite);
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{speed, 0, {850.f, 850.f}, 0.5f});
    registry.add_component(
        _entity, RealEngine::SpriteSheet{
                     _mobSpriteSheet, "fly", 0, {16, 14}, false, true, 55, {8, 8}, sf::Clock()});
    // 55 is the speed of rotation animation
    // registry.add_component(_entity, RealEngine::SpriteComponent{_mobSprite});
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

SpaceSphere::~SpaceSphere() {}

}  // namespace rtype
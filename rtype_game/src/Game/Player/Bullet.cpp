/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Bullet
*/

#include "Game/Player/Bullet.hpp"

namespace rtype {

Bullet::Bullet(RealEngine::Registry& registry, sf::Vector2f position, float speed)
    : _entity(registry.spawn_entity()) {
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(_entity, RealEngine::Velocity{speed, 0, {500.f, 500.f}, 0.f});
    registry.add_component(_entity,
                           RealEngine::SpriteComponent{
                               *(RealEngine::AssetManager::getInstance().getSprite("bullet"))});
    registry.add_component(_entity, RealEngine::Drawable{});
    // registry.add_component(
    //     _entity, RealEngine::Collision{
    //                  {0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
    //                  "bullet",
    //                  false,
    //                  RealEngine::CollisionType::ALLY_BULLET,
    //                  [this](RealEngine::CollisionType collisionType, RealEngine::Registry&
    //                  registry,
    //                         RealEngine::Entity collider, RealEngine::Entity entity) {
    //                      bullet_collision_handler(collisionType, registry, collider, entity);
    //                  }});
    registry.add_component(_entity, RealEngine::AutoDestructible{5});
    registry.add_component(_entity, RealEngine::Damage{10});
    registry.add_component(_entity, RealEngine::Health{10, 20});
}

Bullet::~Bullet() {}
}  // namespace rtype
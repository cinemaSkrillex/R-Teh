/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** SimpleMob
*/

#include "../include/Game/SimpleMob.hpp"

namespace rtype {

SimpleMob::SimpleMob(RealEngine::Registry& registry, sf::Vector2f position, sf::Vector2f direction,
                     float speed, RealEngine::Sprite& mobSprite)
    : _entity(registry.spawn_entity()), _mobSprite(mobSprite) {
    registry.add_component(_entity, RealEngine::Position{position.x, position.y});
    registry.add_component(
        _entity,
        RealEngine::Velocity{direction.x * speed, direction.y * speed, {500.f, 500.f}, 0.f});
    registry.add_component(_entity, RealEngine::SpriteComponent{_mobSprite});
    registry.add_component(_entity, RealEngine::Drawable{});
    registry.add_component(
        _entity,
        RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                              "mob",
                              false,
                              RealEngine::CollisionType::OTHER,
                              [this](RealEngine::CollisionType collisionType,
                                     RealEngine::Registry& registry, RealEngine::Entity collider) {
                                  mob_collision_handler(collisionType, registry, collider);
                              }});
    registry.add_component(_entity, RealEngine::AutoDestructible{5});
    registry.add_component(_entity, RealEngine::Damage{10});
    registry.add_component(_entity, RealEngine::Health{10, 10});
    std::cout << "Mob created id: " << *_entity << std::endl;
}

SimpleMob::~SimpleMob() {}

void SimpleMob::mob_collision_handler(RealEngine::CollisionType collisionType,
                                      RealEngine::Registry& registry, RealEngine::Entity collider) {
    std::cout << "Mob collided" << std::endl;
    switch (collisionType) {
        case RealEngine::CollisionType::INACTIVE:
            break;
        case RealEngine::CollisionType::SOLID:
            break;
        case RealEngine::CollisionType::HIT:
            mob_take_damage(registry, collider);
            break;
        case RealEngine::CollisionType::PICKABLE:
            break;
        default:
            break;
    }
}

void SimpleMob::mob_take_damage(RealEngine::Registry& registry, RealEngine::Entity collider) {
    auto* health = registry.get_component<RealEngine::Health>(_entity);

    if (health) {
        health->damage += 10;
    }
}
}  // namespace rtype
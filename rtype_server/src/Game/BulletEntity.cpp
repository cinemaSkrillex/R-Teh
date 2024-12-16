/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** BulletEntity.cpp
*/

#include "../include/Game/BulletEntity.hpp"

namespace rtype {

// should add if bullet is from player or enemy and modify the collision tag TODO
Bullet::Bullet(RealEngine::Registry& registry, sf::Vector2f position, sf::Vector2f direction,
               float speed, RealEngine::Sprite& bulletSprite)
    : _bulletEntity(registry.spawn_entity()), _bulletSprite(bulletSprite) {
    registry.add_component(_bulletEntity, RealEngine::Position{position.x, position.y});
    registry.add_component(
        _bulletEntity,
        RealEngine::Velocity{direction.x * speed, direction.y * speed, {500.f, 500.f}, 0.f});
    registry.add_component(_bulletEntity, RealEngine::SpriteComponent{_bulletSprite});
    registry.add_component(_bulletEntity, RealEngine::Drawable{});
    registry.add_component(
        _bulletEntity,
        RealEngine::Collision{
            {0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
            "bullet",
            false,
            RealEngine::CollisionType::ALLY_BULLET,
            [this](RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                   RealEngine::Entity collider, RealEngine::Entity entity) {
                bullet_collision_handler(collisionType, registry, collider, entity);
            }});
    registry.add_component(_bulletEntity, RealEngine::AutoDestructible{5});
    registry.add_component(_bulletEntity, RealEngine::Damage{10});
    registry.add_component(_bulletEntity, RealEngine::Health{10, 20});
    std::cout << "Bullet created id: " << *_bulletEntity << std::endl;
}

Bullet::~Bullet() {}

void Bullet::bullet_collision_handler(RealEngine::CollisionType collisionType,
                                      RealEngine::Registry& registry, RealEngine::Entity collider,
                                      RealEngine::Entity entity) {
    switch (collisionType) {
        case RealEngine::CollisionType::INACTIVE:
            break;
        case RealEngine::CollisionType::SOLID:
            bullet_take_damage(registry, collider, entity);
            break;
        case RealEngine::CollisionType::PICKABLE:
            break;
        case RealEngine::CollisionType::OTHER:
            break;
        case RealEngine::CollisionType::PLAYER:
            break;
        case RealEngine::CollisionType::ENEMY:
            // bullet_take_damage(registry, collider, entity);
            break;
        case RealEngine::CollisionType::ALLY_BULLET:
            break;
        case RealEngine::CollisionType::ENEMY_BULLET:
            // bullet_take_damage(registry, collider, entity);
            break;
        default:
            break;
    }
}

void Bullet::bullet_take_damage(RealEngine::Registry& registry, RealEngine::Entity collider,
                                RealEngine::Entity entity) {
    // std::cout << "Bullet take damage" << std::endl;
    // if (_bulletEntity == nullptr) {
    //     std::cout << "Bullet entity is null" << std::endl;
    //     return;
    // }
    // std::cout << "Bullet entity: " << *_bulletEntity << " take damage" << std::endl;
    // auto* health = registry.get_component<RealEngine::Health>(*_bulletEntity);
    auto* health = registry.get_component<RealEngine::Health>(entity);

    if (health) {
        health->damage += 10;
    }
    std::cout << "Bullet took damage" << std::endl;
}
}  // namespace rtype
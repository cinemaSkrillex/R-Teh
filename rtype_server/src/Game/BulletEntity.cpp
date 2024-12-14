/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** BulletEntity.cpp
*/

#include "../include/Game/BulletEntity.hpp"

namespace rtype {

Bullet::Bullet(RealEngine::Registry& registry, sf::Vector2f position, sf::Vector2f direction,
               float speed, RealEngine::Sprite& bulletSprite)
    : _bulletEntity(registry.spawn_entity()), _bulletSprite(bulletSprite) {
    registry.add_component(_bulletEntity, RealEngine::Position{position.x, position.y});
    registry.add_component(_bulletEntity, RealEngine::Velocity{direction.x * speed, direction.y * speed, {500.f, 500.f}, 0.f});
    registry.add_component(_bulletEntity, RealEngine::SpriteComponent{_bulletSprite});
    registry.add_component(_bulletEntity, RealEngine::Drawable{});
    registry.add_component(_bulletEntity, RealEngine::Collision{{0.f, 0.f, 16.f * GAME_SCALE, 8.f * GAME_SCALE},
                                                               "bullet",
                                                               false,
                                                               RealEngine::CollisionType::OTHER,
                                                               [this](RealEngine::CollisionType collisionType,
                                                                      RealEngine::Registry& registry, RealEngine::Entity collider) {
                                                                   bullet_collision_handler(collisionType, registry, collider);
                                                               }});
    registry.add_component(_bulletEntity, RealEngine::AutoDestructible{20});
    registry.add_component(_bulletEntity, RealEngine::Damage{10});
    std::cout << "Bullet created id: " << _bulletEntity << std::endl;
}

Bullet::~Bullet() {}

void Bullet::bullet_collision_handler(RealEngine::CollisionType collisionType,
                                      RealEngine::Registry& registry, RealEngine::Entity collider) {
    std::cout << "Bullet collided" << std::endl;
    switch (collisionType) {
        case RealEngine::CollisionType::INACTIVE:
            break;
        case RealEngine::CollisionType::SOLID:
            registry.remove_entity(_bulletEntity);
            break;
        case RealEngine::CollisionType::HIT:
            try {
                std::cout << "Bullet id: " << _bulletEntity << std::endl;
                registry.remove_entity(_bulletEntity);
            } catch (const std::exception& e) {
                std::cerr << e.what() << '\n';
            }
            break;
        case RealEngine::CollisionType::PICKABLE:
            break;
        default:
            break;
    }
}
}
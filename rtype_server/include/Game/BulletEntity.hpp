/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** BulletEntity.hpp
*/

#pragma once

#include "Engine.hpp"

namespace rtype {
class Bullet {
   public:
    Bullet(RealEngine::Registry& registry, sf::Vector2f position, sf::Vector2f direction,
           float speed, RealEngine::Sprite& bulletSprite);
    ~Bullet();

    RealEngine::Entity getEntity() { return _bulletEntity; }
    void               bullet_collision_handler(RealEngine::CollisionType collisionType,
                                                RealEngine::Registry& registry, RealEngine::Entity collider);

   private:
    RealEngine::Entity _bulletEntity;
    RealEngine::Sprite _bulletSprite;
};
}  // namespace rtype
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

    std::shared_ptr<RealEngine::Entity> getEntity() { return _bulletEntity; }
    void bullet_collision_handler(RealEngine::CollisionType collisionType,
                                  RealEngine::Registry& registry, RealEngine::Entity collider,
                                  RealEngine::Entity entity);
    void bullet_take_damage(RealEngine::Registry& registry, RealEngine::Entity collider,
                            RealEngine::Entity entity);

   private:
    std::shared_ptr<RealEngine::Entity> _bulletEntity;
    RealEngine::Sprite                  _bulletSprite;
};
}  // namespace rtype
/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** SimpleMob
*/

#pragma once

#include "Engine.hpp"

namespace rtype {
class SimpleMob {
   public:
    SimpleMob(RealEngine::Registry& registry, sf::Vector2f position, sf::Vector2f direction,
              float speed, RealEngine::Sprite& mobSprite);
    ~SimpleMob();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }
    void mob_collision_handler(RealEngine::CollisionType collisionType,
                               RealEngine::Registry& registry, RealEngine::Entity collider, RealEngine::Entity entity);
    void mob_take_damage(RealEngine::Registry& registry, RealEngine::Entity collider, RealEngine::Entity entity);

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
    RealEngine::Sprite&                 _mobSprite;
};
}  // namespace rtype
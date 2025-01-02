/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SpacePlane
*/

#pragma once

#include "Engine.hpp"

namespace rtype {
class SpacePlane {
   public:
    SpacePlane(RealEngine::Registry& registry, sf::Vector2f position, sf::Vector2f direction,
               float speed, RealEngine::Sprite& mobSprite);
    ~SpacePlane();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }
    void mob_collision_handler(RealEngine::CollisionType collisionType,
                               RealEngine::Registry& registry, RealEngine::Entity collider,
                               RealEngine::Entity entity);
    void mob_take_damage(RealEngine::Registry& registry, RealEngine::Entity collider,
                         RealEngine::Entity entity);

   private:
    std::shared_ptr<RealEngine::Entity>                 _entity;
    RealEngine::Sprite&                                 _mobSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _mobSpriteSheet;
};
}  // namespace rtype
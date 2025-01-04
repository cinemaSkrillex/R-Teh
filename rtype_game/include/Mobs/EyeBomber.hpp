/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeBomber
*/

#pragma once

#include "Engine.hpp"

namespace rtype {

// this enemy accelerate towards the player and explodes on contact

class EyeBomber {
   public:
    EyeBomber(RealEngine::Registry& registry, sf::Vector2f position, sf::Vector2f direction,
              float speed);
    ~EyeBomber();
    std::shared_ptr<RealEngine::Entity> getEntity() { return _eyeEntity; }
    void setTarget(std::shared_ptr<RealEngine::Entity> target, RealEngine::Registry& registry);
    void collisionBehaviour(RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                            RealEngine::Entity collider, RealEngine::Entity entity);

   private:
    std::shared_ptr<RealEngine::Entity>                 _eyeEntity;
    RealEngine::Sprite                                  _eyeSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _eyeSheet;
};
}  // namespace rtype
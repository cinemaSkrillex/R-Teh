/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RobotMiniBoss
*/

#pragma once

#include "Engine.hpp"

namespace rtype {
class RobotMiniBoss {
   public:
    RobotMiniBoss(RealEngine::Registry& registry, sf::Vector2f position, sf::Vector2f direction,
                  float speed);
    ~RobotMiniBoss();
    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }
    void setTarget(std::shared_ptr<RealEngine::Entity> target, RealEngine::Registry& registry);
    void collisionBehaviour(RealEngine::CollisionType collisionType, RealEngine::Registry& registry,
                            RealEngine::Entity collider, RealEngine::Entity entity);

   private:
    std::shared_ptr<RealEngine::Entity>                 _entity;
    RealEngine::Sprite                                  _shootMobSprite;
    RealEngine::Sprite                                  _fordwardMobSprite;
    RealEngine::Sprite                                  _backwardMobSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _mobSpriteSheet;
};
}  // namespace rtype
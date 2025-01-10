/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RobotMiniBoss
*/

#pragma once

#include "Game/Mobs/Behaviors.hpp"

namespace rtype {
class RobotMiniBoss {
   public:
    RobotMiniBoss(RealEngine::Registry& registry, sf::Vector2f position);
    ~RobotMiniBoss();
    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity>                 _entity;
    RealEngine::Sprite                                  _shootMobSprite;
    RealEngine::Sprite                                  _fordwardMobSprite;
    RealEngine::Sprite                                  _backwardMobSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _mobSpriteSheet;
};
}  // namespace rtype
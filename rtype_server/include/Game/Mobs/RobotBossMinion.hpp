/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** RobotBossMinion
*/

#pragma once

#include "Game/Mobs/Behaviors.hpp"
#include "Game/Mobs/Projectiles/SpaceMissile.hpp"

namespace rtype {
class RobotBossMinion {
   public:
    RobotBossMinion(RealEngine::Registry& registry, sf::Vector2f position);
    ~RobotBossMinion();
    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity>                 _entity;
    RealEngine::Sprite                                  _mobSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _mobSpriteSheet;
};
}  // namespace rtype
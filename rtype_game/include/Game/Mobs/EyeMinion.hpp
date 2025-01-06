/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeMinion
*/

#pragma once

#include "Game/Mobs/Behaviors.hpp"
#include "Game/Mobs/Projectiles/SmallLaser.hpp"

namespace rtype {

// this enemy have some boosts towards the player and shoot projectiles

class EyeMinion {
   public:
    EyeMinion(RealEngine::Registry& registry, sf::Vector2f position, sf::Vector2f direction,
              float speed);
    ~EyeMinion();
    std::shared_ptr<RealEngine::Entity> getEntity() { return _eyeEntity; }
    void setTarget(std::shared_ptr<RealEngine::Entity> target, RealEngine::Registry& registry);

   private:
    std::shared_ptr<RealEngine::Entity>                 _eyeEntity;
    RealEngine::Sprite                                  _eyeSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _eyeSheet;
};
}  // namespace rtype
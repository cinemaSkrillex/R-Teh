/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** MiniBossShoot
*/

#pragma once

#include "Engine.hpp"
#include "Game/Mobs/Behaviors.hpp"

namespace rtype {

class MiniBossShoot {
   public:
    MiniBossShoot(RealEngine::Registry& registry, sf::Vector2f position, float angle, float speed);
    ~MiniBossShoot();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity>                 _entity;
    RealEngine::Sprite                                  _projSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _projSpriteSheet;
};
}  // namespace rtype
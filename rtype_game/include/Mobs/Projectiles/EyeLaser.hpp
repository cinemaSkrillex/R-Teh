/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeLaser
*/

#pragma once

#include "Engine.hpp"
#include "Mobs/Behaviors.hpp"

namespace rtype {

class EyeLaser {
   public:
    EyeLaser(RealEngine::Registry& registry, sf::Vector2f position, float angle, float speed);
    ~EyeLaser();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity>                 _entity;
    RealEngine::Sprite                                  _projSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _projSpriteSheet;
};
}  // namespace rtype
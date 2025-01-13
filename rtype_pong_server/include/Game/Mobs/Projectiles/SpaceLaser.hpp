/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SpaceLaser
*/

#pragma once

#include "Game/Mobs/Behaviors.hpp"

namespace rtype {

class SpaceLaser {
   public:
    SpaceLaser(RealEngine::Registry& registry, sf::Vector2f position, float angle);
    ~SpaceLaser();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity>                 _entity;
    RealEngine::Sprite                                  _projSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _projSpriteSheet;
};
}  // namespace rtype
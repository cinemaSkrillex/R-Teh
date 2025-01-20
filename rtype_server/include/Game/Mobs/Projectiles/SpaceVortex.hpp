/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SpaceVortex
*/

#pragma once

#include "Game/Mobs/Behaviors.hpp"

namespace rtype {

class SpaceVortex {
   public:
    SpaceVortex(RealEngine::Registry& registry, sf::Vector2f position);
    ~SpaceVortex();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity>                 _entity;
    RealEngine::Sprite                                  _projSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _projSpriteSheet;
};
}  // namespace rtype
/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SpacePlane
*/

#pragma once

#include "Game/Mobs/Behaviors.hpp"

// this enemy has basic movements with different patterns and shoots straight bullets

namespace rtype {
class SpacePlane {
   public:
    SpacePlane(RealEngine::Registry& registry, sf::Vector2f position);
    ~SpacePlane();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity>                 _entity;
    RealEngine::Sprite                                  _mobSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _mobSpriteSheet;
};
}  // namespace rtype
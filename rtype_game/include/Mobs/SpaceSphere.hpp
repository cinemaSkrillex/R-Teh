/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SpaceSphere
*/

#pragma once

#include "Engine.hpp"

namespace rtype {
class SpaceSphere {
   public:
    SpaceSphere(RealEngine::Registry& registry, sf::Vector2f position, sf::Vector2f direction,
                float speed, RealEngine::Sprite& mobSprite);
    ~SpaceSphere();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity>                 _entity;
    RealEngine::Sprite&                                 _mobSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _mobSpriteSheet;
};
}  // namespace rtype
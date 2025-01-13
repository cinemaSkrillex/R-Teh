/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SpaceBomb
*/

#pragma once

#include "Engine.hpp"

namespace rtype {

class SpaceBomb {
   public:
    SpaceBomb(RealEngine::Registry& registry, sf::Vector2f position, float angle, float speed);
    ~SpaceBomb();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
    RealEngine::Sprite                  _projSprite;
};
}  // namespace rtype
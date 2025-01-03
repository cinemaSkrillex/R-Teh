/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** MortarRocket
*/

#pragma once

#include "Engine.hpp"

namespace rtype {

class MortarRocket {
   public:
    MortarRocket(RealEngine::Registry& registry, sf::Vector2f position, float angle, float speed,
                 RealEngine::Sprite& projSprite);
    ~MortarRocket();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
    RealEngine::Sprite                  _projSprite;
};
}  // namespace rtype
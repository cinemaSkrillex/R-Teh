/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Background
*/

#pragma once

#include "Engine.hpp"

namespace rtype {
class Background {
   public:
    Background(RealEngine::Registry& registry, float speed, int scale);
    ~Background();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
    RealEngine::Sprite                  _backgroundSprite;
};
}  // namespace rtype
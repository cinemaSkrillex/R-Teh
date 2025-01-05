/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Bullet
*/

#pragma once

#include "Engine.hpp"

namespace rtype {

class Bullet {
   public:
    Bullet(RealEngine::Registry& registry, sf::Vector2f position, float speed);
    ~Bullet();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity>                 _entity;
    std::unordered_map<std::string, RealEngine::Sprite> _projSpriteSheet;
};

}  // namespace rtype
/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** ExplosiveShoot
*/

#pragma once

#include "Engine.hpp"

namespace rtype {

class ExplosiveShoot {
   public:
    ExplosiveShoot(RealEngine::Registry& registry, sf::Vector2f position, float angle, float speed);
    ~ExplosiveShoot();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity>                 _entity;
    RealEngine::Sprite                                  _projSprite;
    std::unordered_map<std::string, RealEngine::Sprite> _projSpriteSheet;
};
}  // namespace rtype
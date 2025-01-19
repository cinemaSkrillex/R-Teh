/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SmallLaser
*/

#pragma once

#include "Game/Mobs/Behaviors.hpp"

namespace rtype {

class SmallLaser {
   public:
    SmallLaser(RealEngine::Registry& registry, sf::Vector2f position, float angle);
    ~SmallLaser();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
};
}  // namespace rtype
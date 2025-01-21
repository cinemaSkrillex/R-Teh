/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SimpleShoot
*/

#pragma once

#include "Engine.hpp"
#include "Game/Mobs/Behaviors.hpp"

namespace rtype {

class SimpleShoot {
   public:
    SimpleShoot(RealEngine::Registry& registry, sf::Vector2f position, float angle);
    ~SimpleShoot();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
};
}  // namespace rtype
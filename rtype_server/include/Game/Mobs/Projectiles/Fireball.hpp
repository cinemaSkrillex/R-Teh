/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Fireball
*/

#pragma once

#include "Game/Mobs/Behaviors.hpp"

namespace rtype {

class Fireball {
   public:
    Fireball(RealEngine::Registry& registry, sf::Vector2f position, float angle);
    ~Fireball();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
};
}  // namespace rtype
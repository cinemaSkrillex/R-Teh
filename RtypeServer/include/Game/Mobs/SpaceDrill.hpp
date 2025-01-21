/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SpaceDrill
*/

#pragma once

#include "Game/Mobs/Behaviors.hpp"

namespace rtype {

// this enemy move up and down with some fast fordward boosts and return to its base position

class SpaceDrill {
   public:
    SpaceDrill(RealEngine::Registry& registry, sf::Vector2f position);
    ~SpaceDrill();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
};
}  // namespace rtype
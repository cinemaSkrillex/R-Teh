/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** SpaceMissile
*/

#pragma once

#include "Engine.hpp"
#include "Game/Mobs/Behaviors.hpp"

namespace rtype {

class SpaceMissile {
   public:
    SpaceMissile(RealEngine::Registry& registry, sf::Vector2f position, float angle);
    ~SpaceMissile();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
};
}  // namespace rtype
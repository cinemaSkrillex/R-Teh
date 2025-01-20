/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** PowerUp
*/

#pragma once

#include "Engine.hpp"
#include "Game/Mobs/Behaviors.hpp"

namespace rtype {
enum bonusType { HEAL, SHOOT, SPEED };

class PowerUp {
   public:
    PowerUp(RealEngine::Registry& registry, sf::Vector2f pos, bonusType type);
    ~PowerUp();

    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
};
}  // namespace rtype
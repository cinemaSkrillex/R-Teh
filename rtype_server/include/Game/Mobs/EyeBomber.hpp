/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** EyeBomber
*/

#pragma once

#include "Game/Mobs/Behaviors.hpp"

namespace rtype {

class EyeBomber {
   public:
    EyeBomber(RealEngine::Registry& registry, sf::Vector2f position);
    ~EyeBomber();
    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }
    void setTarget(std::shared_ptr<RealEngine::Entity> target, RealEngine::Registry& registry);

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
};
}  // namespace rtype
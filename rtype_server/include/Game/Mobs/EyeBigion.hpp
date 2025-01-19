/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeBigion
*/

#pragma once

#include "Game/Mobs/Behaviors.hpp"
#include "Game/Mobs/EyeBomber.hpp"

namespace rtype {

// this enemy move slowly towards the player sometimes shoot projectiles, and sometimes make eye
// bomber spawn. It have a lot of health

class EyeBigion {
   public:
    EyeBigion(RealEngine::Registry& registry, sf::Vector2f position);
    ~EyeBigion();
    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }
    void setTarget(std::shared_ptr<RealEngine::Entity> target, RealEngine::Registry& registry);

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
};
}  // namespace rtype
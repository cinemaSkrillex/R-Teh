/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** EyeBoss
*/

#pragma once

#include "Game/Mobs/Behaviors.hpp"
#include "Game/Mobs/Projectiles/EyeLaser.hpp"

namespace rtype {

class EyeBoss {
   public:
    EyeBoss(RealEngine::Registry& registry, sf::Vector2f position);
    ~EyeBoss();
    void setTarget(RealEngine::Registry& registry, std::shared_ptr<RealEngine::Entity> target);
    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
};
}  // namespace rtype
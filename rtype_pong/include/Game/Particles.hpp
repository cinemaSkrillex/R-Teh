/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Particles
*/

#pragma once

#include "Engine.hpp"

namespace rtype {
class Explosion {
   public:
    Explosion(RealEngine::Registry& registry, sf::Vector2f position);
    ~Explosion();
    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
};

class HitEffect {
   public:
    HitEffect(RealEngine::Registry& registry, sf::Vector2f position);
    ~HitEffect();
    std::shared_ptr<RealEngine::Entity> getEntity() { return _entity; }

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
};
}  // namespace rtype

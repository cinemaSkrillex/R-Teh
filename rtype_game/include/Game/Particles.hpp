/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Particles
*/

#pragma once

#include "Engine.hpp"

namespace rtype {

enum ParticleType { EXPLOSION, HIT_EFFECT, SHOOT_LOAD };

class ParticlesClass {
   public:
    ParticlesClass(sf::Vector2f position, ParticleType type);
    ~ParticlesClass();
    RealEngine::ParticleEmitter create_explosion(sf::Vector2f position);
    RealEngine::ParticleEmitter create_hit_effect(sf::Vector2f position);
    RealEngine::ParticleEmitter create_shoot_load(sf::Vector2f position);
    RealEngine::ParticleEmitter create_shoot_destroy(sf::Vector2f position);
    RealEngine::ParticleEmitter getParticle() { return _particle; }

   private:
    RealEngine::ParticleEmitter _particle;
};

class Explosion {
   public:
    Explosion(RealEngine::Registry& registry, sf::Vector2f position);
    ~Explosion();

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
};

class HitEffect {
   public:
    HitEffect(RealEngine::Registry& registry, sf::Vector2f position);
    ~HitEffect();

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
};

class ShootLoad {
   public:
    ShootLoad(RealEngine::Registry& registry, sf::Vector2f position);
    ~ShootLoad();

   private:
    std::shared_ptr<RealEngine::Entity> _entity;
};

}  // namespace rtype

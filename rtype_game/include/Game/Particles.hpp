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

}  // namespace rtype

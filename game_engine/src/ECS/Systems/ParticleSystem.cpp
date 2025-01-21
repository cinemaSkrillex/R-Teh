/*
** EPITECH PROJECT, 2024
** r-teh-mega-backup
** File description:
** ParticleSystem
*/

// R-Teh/game_engine/src/ECS/Systems/ParticleSystem.cpp
#include "ECS/Systems/ParticleSystem.hpp"

namespace RealEngine {

void ParticleSystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<ParticleEmitter>();

    for (auto entity : entities) {
        auto* emitter = registry.getComponent<ParticleEmitter>(entity);

        // Update elapsed time
        emitter->elapsedTime += deltaTime;

        // Emit new particles if within emission duration
        if (emitter->elapsedTime < emitter->emissionDuration) {
            for (int i = 0; i < emitter->emissionRate.x * deltaTime; ++i) {
                Particle particle;
                particle.position = emitter->position;
                particle.velocity = {static_cast<float>(rand() % 100 - 50),
                                     static_cast<float>(rand() % 100 - 50)};
                particle.color    = emitter->startColor;
                particle.lifetime = emitter->particleLifetime;
                particle.size     = emitter->particleSize;
                emitter->particles.push_back(particle);
            }
        }

        // Update existing particles
        for (auto& particle : emitter->particles) {
            particle.position += particle.velocity * deltaTime;
            particle.lifetime -= deltaTime;
            float lifeRatio = particle.lifetime / emitter->particleLifetime;
            particle.color =
                sf::Color(static_cast<sf::Uint8>(emitter->endColor.r * (1 - lifeRatio) +
                                                 emitter->startColor.r * lifeRatio),
                          static_cast<sf::Uint8>(emitter->endColor.g * (1 - lifeRatio) +
                                                 emitter->startColor.g * lifeRatio),
                          static_cast<sf::Uint8>(emitter->endColor.b * (1 - lifeRatio) +
                                                 emitter->startColor.b * lifeRatio),
                          static_cast<sf::Uint8>(emitter->endColor.a * (1 - lifeRatio) +
                                                 emitter->startColor.a * lifeRatio));
        }

        // Remove dead particles
        emitter->particles.erase(
            std::remove_if(emitter->particles.begin(), emitter->particles.end(),
                           [](const Particle& p) { return p.lifetime <= 0; }),
            emitter->particles.end());

        // Destroy entity if emission duration has passed and no particles are left
        // if (emitter->elapsedTime >= emitter->emissionDuration && emitter->particles.empty()) {
        //     registry.killEntity(entity);
        // }
    }
}

}  // namespace RealEngine
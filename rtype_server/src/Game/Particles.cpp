/*
** EPITECH PROJECT, 2025
** R-Teh
** File description:
** Particles
*/

#include "Game/Particles.hpp"

namespace rtype {

Explosion::Explosion(RealEngine::Registry &registry, sf::Vector2f position)
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity, RealEngine::ParticleEmitter{{},
                                                               {position.x, position.y},
                                                               {10, 0},
                                                               2.0f,
                                                               5.0f,
                                                               sf::Color::White,
                                                               sf::Color::Transparent,
                                                               2.0f,
                                                               0.0f});
}

Explosion::~Explosion() {}

HitEffect::HitEffect(RealEngine::Registry &registry, sf::Vector2f position)
    : _entity(registry.spawnEntity()) {
    registry.addComponent(_entity, RealEngine::ParticleEmitter{{},
                                                               {position.x, position.y},
                                                               {1, 0},
                                                               2.0f,
                                                               5.0f,
                                                               sf::Color::White,
                                                               sf::Color::Transparent,
                                                               0.2f,
                                                               0.0f});
}

HitEffect::~HitEffect() {}

}  // namespace rtype
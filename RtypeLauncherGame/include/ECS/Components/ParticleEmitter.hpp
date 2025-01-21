/*
** EPITECH PROJECT, 2024
** r-teh-mega-backup
** File description:
** ParticleEmitter
*/

#ifndef PARTICLEEMITTER_HPP_
#define PARTICLEEMITTER_HPP_

// R-Teh/GameEngine/include/ECS/Components/ParticleEmitter.hpp
#pragma once

#include <vector>

#include "Particle.hpp"

namespace RealEngine {
struct ParticleEmitter {
    std::vector<Particle> particles;
    sf::Vector2f          position;
    sf::Vector2f          emissionRate;
    float                 particleLifetime;
    float                 particleSize;
    sf::Color             startColor;
    sf::Color             endColor;
    float                 emissionDuration;
    float                 elapsedTime;
};
}  // namespace RealEngine

#endif /* !PARTICLEEMITTER_HPP_ */

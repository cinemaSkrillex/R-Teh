/*
** EPITECH PROJECT, 2024
** r-teh-mega-backup
** File description:
** Particle
*/

#ifndef PARTICLE_HPP_
#define PARTICLE_HPP_

// R-Teh/game_engine/include/ECS/Components/Particle.hpp
#pragma once

#include <SFML/Graphics.hpp>

namespace RealEngine {
struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color    color;
    float        lifetime;
    float        size;
};
}  // namespace RealEngine

#endif /* !PARTICLE_HPP_ */

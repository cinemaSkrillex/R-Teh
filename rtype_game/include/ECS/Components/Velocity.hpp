/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Velocity.hpp
*/

#pragma once

#include <SFML/System/Vector2.hpp>

namespace RealEngine {
struct Velocity {
    float        vx, vy;
    sf::Vector2f maxSpeed;
    float        airFrictionForce = 0.0f;
};
}  // namespace RealEngine
/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Interpolation
*/

#pragma once

#include <SFML/System/Vector2.hpp>

namespace RealEngine {
struct Interpolation {
    sf::Vector2f start;
    sf::Vector2f end;
    float        step;
    float        currentStep;
    bool         reset;
};
}  // namespace RealEngine
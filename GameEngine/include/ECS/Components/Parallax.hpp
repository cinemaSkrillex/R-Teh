/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Parallax
*/

#pragma once

#include <SFML/System/Vector2.hpp>

namespace RealEngine {
struct Parallax {
    // set speed multiplier for parallax effect
    float speed;
    // base size of the parallax object before scaling
    sf::Vector2f size;
    bool         initialized = false;
};
}  // namespace RealEngine
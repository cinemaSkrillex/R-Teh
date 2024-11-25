/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Collision
*/

#pragma once

#include <SFML/Graphics.hpp>

struct Collision {
    sf::FloatRect bounds;
    std::string id;
    bool isColliding;
    // sf::Vector2f size; for later.
};

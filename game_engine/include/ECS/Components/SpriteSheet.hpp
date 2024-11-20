/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** SpriteSheet
*/

#pragma once

#include "Sprite.hpp"

struct SpriteSheet {
    std::unordered_map<std::string, RealEngine::Sprite> sprites;
    std::string                                         spriteIndex; // current selected sprite.
    int                                                 frameIndex;  // current rendered frame.
    sf::Vector2i frameSize; // size of a single frame from the spriteSheet.
    bool         pause;
    bool         loop;
    float        animTime; // Time in milliseconds before updating the frame.
};
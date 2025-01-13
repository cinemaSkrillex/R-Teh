/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** SpriteSheet
*/

#pragma once

#include <unordered_map>

#include "Sprite.hpp"

namespace RealEngine {
struct SpriteSheet {
    // TODO maybe: add a pair of RealEngine::Sprite x struct for more info on each spritesheet to
    // allow more flexibility template for the std::unordered map cuz that bit** takes 10mins to
    // write down.
    std::unordered_map<std::string, RealEngine::Sprite> sprites;
    std::string                                         spriteIndex;  // current selected sprite.
    int                                                 frameIndex;   // current rendered frame.
    sf::Vector2i frameSize;  // size of a single frame from the spriteSheet.
    bool         pause;
    bool         loop;
    float        animTime;  // Time in milliseconds before updating the frame.
    sf::Vector2i origin = {-1, -1};
    sf::Clock    animClock;
};
}  // namespace RealEngine
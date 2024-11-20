/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** SpriteSheet
*/

#pragma once

#include "Sprite.hpp"

struct SpriteSheet {
    Sprite sprite;
    float animTime; // Time in milliseconds before updating the frame.
};
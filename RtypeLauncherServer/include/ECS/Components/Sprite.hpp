/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Sprite
*/

#pragma once

#include "Core/Graphics/Rendering/Sprite.hpp"

namespace RealEngine {
struct SpriteComponent {
    RealEngine::Sprite sprite;
    int                zIndex = 0;
};
}  // namespace RealEngine
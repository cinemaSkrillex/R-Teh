/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** Velocity.hpp
*/

#pragma once

namespace RealEngine {
struct Velocity {
    float vx, vy;
    float airFrictionForce = 0.0f;
};
}  // namespace RealEngine
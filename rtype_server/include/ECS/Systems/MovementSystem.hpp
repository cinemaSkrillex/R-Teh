/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** MovementSystem.hpp
*/

#pragma once
#include <cmath>

#include "../include/ECS/Components/Position.hpp"
#include "../include/ECS/Components/Velocity.hpp"
#include "../include/ECS/Registry/Registry.hpp"

namespace RealEngine {
class MovementSystem {
   public:
    void update(Registry& registry, float deltaTime);
    void limitSpeed(Velocity& velocity);
    void applyFriction(Velocity& velocity, float deltaTime);
};
}  // namespace RealEngine
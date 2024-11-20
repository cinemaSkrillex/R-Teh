/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** MovementSystem.hpp
*/

#pragma once
#include "../include/ECS/Registry/Registry.hpp"
#include "../include/ECS/Components/Position.hpp"
#include "../include/ECS/Components/Velocity.hpp"

class MovementSystem {
  public:
    void update(Registry& registry, SparseArray<Position>& positions,
                SparseArray<Velocity>& velocities, float deltaTime);
};
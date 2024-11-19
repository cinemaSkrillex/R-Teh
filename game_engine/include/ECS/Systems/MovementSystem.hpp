/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** MovementSystem.hpp
*/

#pragma once
#include "Registry.hpp"
#include "Position.hpp"
#include "Velocity.hpp"

class MovementSystem {
  public:
    void update(Registry& registry, SparseArray<Position>& positions,
                SparseArray<Velocity>& velocities, float deltaTime);
};
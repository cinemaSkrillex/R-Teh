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
                SparseArray<Velocity>& velocities, float deltaTime) {

        for (std::size_t i = 0; i < positions.size(); ++i) {
            if (positions[i] && velocities[i]) {
                positions[i]->x += velocities[i]->vx * deltaTime;
                positions[i]->y += velocities[i]->vy * deltaTime;
            }
        }
    }
};
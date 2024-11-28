/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** MovementSystem.cpp
*/

#include "../include/ECS/Systems/MovementSystem.hpp"

namespace RealEngine {

void MovementSystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<Position, Velocity>();

    for (auto entity : entities) {
        auto* position = registry.get_component<Position>(entity);
        auto* velocity = registry.get_component<Velocity>(entity);
        if (position && velocity) {
            position->x += velocity->vx * deltaTime;
            position->y += velocity->vy * deltaTime;
        }
    }
}
} // namespace RealEngine

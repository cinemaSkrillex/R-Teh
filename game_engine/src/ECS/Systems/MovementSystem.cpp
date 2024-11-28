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
        if (velocity->airFrictionForce > 0.0f) {
            applyFriction(*velocity, deltaTime);
        }
    }
}

void MovementSystem::applyFriction(Velocity& velocity, float deltaTime) {
    float speed = std::sqrt(velocity.vx * velocity.vx + velocity.vy * velocity.vy);

    if (speed > 0.0f) {
        float friction = velocity.airFrictionForce * speed * deltaTime;
        velocity.vx -= friction * (velocity.vx / speed);
        velocity.vy -= friction * (velocity.vy / speed);
        if (std::abs(velocity.vx) < 0.01f) velocity.vx = 0.0f;
        if (std::abs(velocity.vy) < 0.01f) velocity.vy = 0.0f;
    }
}
}  // namespace RealEngine

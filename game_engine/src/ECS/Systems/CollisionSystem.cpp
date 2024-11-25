/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** CollisionSystem
*/

#include "../include/ECS/Systems/CollisionSystem.hpp"

CollisionSystem::CollisionSystem() {}

bool isCollidingWithOthers(std::optional<Collision> collision, SparseArray<Collision>& collisions) {
    for (auto currentCollision : collisions) {
        if (collision->bounds.intersects(currentCollision->bounds))
            return true;
    }
    return false;
}

void CollisionSystem::update(Registry& registry, SparseArray<Collision>& collisions,
                             float deltaTime) {
    for (auto collision : collisions) {
        collision->isColliding = isCollidingWithOthers(collision, collisions);
    }
}
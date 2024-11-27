/*
** EPITECH PROJECT, 2024
** R-Teh
** File description:
** CollisionSystem
*/

#include "../include/ECS/Systems/CollisionSystem.hpp"

namespace RealEngine {

CollisionSystem::CollisionSystem() {}

bool isCollidingWithOthers(std::optional<Collision> collision, SparseArray<Collision>& collisions) {
    for (auto currentCollision : collisions) {
        if (currentCollision->id.compare(collision->id) == 0)
            continue;
        if (collision->bounds.intersects(currentCollision->bounds))
            return true;
    }
    return false;
}

void CollisionSystem::update(Registry& registry, SparseArray<Collision>& collisions,
                             SparseArray<SpriteComponent>& sprites,
                             SparseArray<SpriteSheet>& spritesheets, float deltaTime) {
    for (std::size_t i = 0; i < collisions.size(); ++i) {
        if (sprites[i])
            collisions[i]->bounds = sprites[i]->sprite.getBounds();

        if (spritesheets[i])
            collisions[i]->bounds =
                spritesheets[i]->sprites.at(spritesheets[i]->spriteIndex).getBounds();
    }

    for (auto collision : collisions) {
        // std::cout << "pos: " << collision->bounds.left << " " << collision->bounds.top
        //           << " | size: " << collision->bounds.width << " " << collision->bounds.height
        //           << std::endl;
        collision->isColliding = isCollidingWithOthers(collision, collisions);
    }
}
} // namespace RealEngine
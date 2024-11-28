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

void CollisionSystem::update(Registry& registry, float deltaTime) {
    auto entities = registry.view<Collision, SpriteComponent, SpriteSheet>();

    for (auto entity : entities) {
        auto* collision   = registry.get_component<Collision>(entity);
        auto* sprite      = registry.get_component<SpriteComponent>(entity);
        auto* spritesheet = registry.get_component<SpriteSheet>(entity);
        if (sprite)
            collision->bounds = sprite->sprite.getBounds();
        if (spritesheet)
            collision->bounds = spritesheet->sprites.at(spritesheet->spriteIndex).getBounds();
        collision->isColliding =
            isCollidingWithOthers(*collision, registry.get_components<Collision>());
    }
}
} // namespace RealEngine